import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib as mpl
import matplotlib.pyplot as plt
import math
import sys

from mpi4py import MPI

if __name__ == "__main__":
    a = 0.25; T = 2.0; X = 100.0
    X_original = X
    fi = lambda x: math.sin(x/10)  #t=0
    ksi = lambda t: t**3#x=0
    f = lambda t, x: math.sin(x*t/2) + t**3
    
    
    h = h_original = 0.01
    tau = tau_original = 0.01
    # tau = h/a 
    # tau = 1.2 * h/a 
    comm = MPI.COMM_WORLD
    size = comm.Get_size()
    rank = comm.Get_rank()
    est_time = []
    nodes_amount = []
    # y = [] 
    # t =[] 
    # comp_per_unit = 0
    # last_unit_comp = 0
    if size != 1:
        if sys.argv[1] == 'speedup':
            repeat_times = 10
        else:
            repeat_times = 1

        transfer_time = 0
        for _ in range(repeat_times):
            X = X_original
            X += h
            nodes_amount.append((X/h)*(T/tau))
            comp_per_unit = (X // h) // size #computations per unit
            if X // h % size == 0:
                last_unit_comp = comp_per_unit + 1 #last unit computations
            else:
                last_unit_comp = (X // h) - comp_per_unit * (size-1) + 1
            t = np.arange(0, T + tau, tau)

            if rank != size - 1:
                x = np.arange(rank * comp_per_unit * h, (rank + 1) * comp_per_unit * h - h*0.0000001, h)#crashed here smtimes
            else:
                x = np.arange(rank * comp_per_unit * h, (rank * comp_per_unit + last_unit_comp) * h, h)

            y = [0] * len(t)
            for i in range(len(t)):
                y[i] = [0.] * len(x)

            for j in range(0, len(x)):
                y[0][j] = fi(x[j]) # edge conditions x: u(0,x) = fi(x)
            

            if rank != 0 and rank != size - 1:
                for i in range(0, len(t) - 1):
                    left_border = comm.recv(source=rank-1, tag=1) # left border
                    comm.send(y[i][len(x) - 1], dest=rank+1, tag=1)
                    right_border = comm.recv(source=rank+1, tag=0) # right border
                    comm.send(y[i][0], dest=rank-1, tag=0)
                    y[i+1][0] = 0.5*(y[i][1] + left_border) - 0.5*(tau/h)*(y[i][1] - left_border) + tau*f(t[i], x[0])#mb err
                    for j in range(1, len(x) - 1):
                        y[i+1][j] = 0.5*(y[i][j+1] + y[i][j-1]) - 0.5*(tau/h)*(y[i][j+1] - y[i][j-1]) + tau*f(t[i], x[j])
                    y[i+1][len(x) - 1] = 0.5*(right_border + y[i][len(x)-2]) - 0.5*(tau/h)*(right_border - y[i][len(x)-2]) + tau*f(t[i], x[len(x) - 1])
                    y = np.array(y)
                comm.send(y, dest = size - 1, tag=2)
                # print("rank: {} time: {} rcvd from b: {} rcvd from s: {} send to b: {} send to s: {}".format(rank, ))


            if rank == 0:
                for i in range(0, len(t)):
                    y[i][0] = ksi(t[i])
                comp_time = 0
                for i in range(0, len(t) - 1):
                    tr_s = MPI.Wtime()
                    comm.send(y[i][len(x) - 1], dest=1, tag=1)
                    right_border = comm.recv(source=1, tag=0)
                    transfer_time = MPI.Wtime() - tr_s
                    start = MPI.Wtime()
                    for j in range(1, len(x) - 1):
                        y[i+1][j] = 0.5*(y[i][j+1] + y[i][j-1]) - 0.5*(tau/h)*(y[i][j+1] - y[i][j-1]) + tau*f(t[i], x[j])
                    y[i+1][len(x) - 1] = 0.5*(right_border + y[i][len(x)-2]) - 0.5*(tau/h)*(right_border - y[i][len(x)-2]) + tau*f(t[i], x[len(x) - 1])
                    comp_time += MPI.Wtime() - start
                y = np.array(y)
                est_time.append(comp_time)
                print("start time: {}".format(comp_time))
                comm.send(y, dest=size-1, tag=2)

            if rank == size - 1:
                for i in range(0, len(t) - 1):
                    left_border = comm.recv(source=rank-1, tag=1)
                    comm.send(y[i][0], dest=rank-1, tag=0)
                    y[i+1][0] = 0.5*(y[i][1] + left_border) - 0.5*(tau/h)*(y[i][1] - left_border) + tau*f(t[i], x[0])
                    for j in range(1, len(x) - 1):
                        y[i+1][j] = 0.5*(y[i][j+1] + y[i][j-1]) - 0.5*(tau/h)*(y[i][j+1] - y[i][j-1]) + tau*f(t[i], x[j])
                    y[i+1][len(x) - 1] = y[i][len(x) - 1] - tau*(y[i][len(x) - 1]-y[i][len(x) - 2])/h + tau*f(t[i], x[j])
                y = np.array(y)
                for i in range(size - 2, -1, -1):
                    recieved_data = comm.recv(source=i, tag=2)
                    y = np.concatenate((recieved_data, y), axis=1)

            h *= 0.93
            tau *= 0.93




        if rank == size - 1 and sys.argv[1] == 'plot':
            print("cpu: ", comp_per_unit, " luc: ", last_unit_comp)
            fig3 = plt.figure()
            ax3 = fig3.add_subplot(111, projection='3d')
            x = np.arange(0, ((size - 1) * comp_per_unit + last_unit_comp) * h_original, h_original)
            x, t = np.meshgrid(x, t)
            print('x: ',x.shape,' t: ', t.shape, ' y: ', y.shape )
            ax3.plot_surface(x, t, y)

            ax3.set_xlabel('plot for %d workers' % size)
            ax3.set_xlabel('x')
            ax3.set_ylabel('t')
            ax3.set_zlabel('u')
            # plt.show()
            plt.savefig('%dwplot.png' % (size))


        if rank == 0 and sys.argv[1] == 'speedup':
            fig3 = plt.figure()
            ax3 = fig3.add_subplot(111)
            x = np.array(nodes_amount)
            y = np.array(est_time)
            # ax3.set_xscale('log')
            # ax3.set_yscale('log')
            ax3.plot(x, y)
            ax3.set_title('speedup for %d workers\nwith ttpn (ms): %f\nestimated time: %f' % (size, transfer_time / (1000*(h/0.93)*(tau/0.93)), est_time[0]))
            ax3.set_xlabel('nodes amount')
            ax3.set_ylabel('time estimated')
            # plt.show()
            plt.savefig('%dwspeedup.png' % (size))
    else:
        x = np.arange(0, X + h, h)
        t = t = np.arange(0, T + tau, tau)
        y = [0] * len(t)
        start = MPI.Wtime()
        for i in range(len(t)):
            y[i] = [0.] * len(x)
            y[i][0] = ksi(t[i])
        for j in range(0, len(x)):
            y[0][j] = fi(x[j]) # edge conditions x: u(0,x) = fi(x)
        for i in range(0, len(t) - 1):
                    for j in range(1, len(x) - 1):
                        y[i+1][j] = 0.5*(y[i][j+1] + y[i][j-1]) - 0.5*(tau/h)*(y[i][j+1] - y[i][j-1]) + tau*f(t[i], x[j])
                    y[i+1][len(x) - 1] = y[i][len(x) - 1] - tau*(y[i][len(x) - 1]-y[i][len(x) - 2])/h + tau*f(t[i], x[j])
        finish = MPI.Wtime()
        fig = plt.figure()
        ax = fig.add_subplot(111,projection='3d')
        x, t = np.meshgrid(x, t)
        u = np.array(y)
        ax.plot_surface(x,t,u)
        ax.set_title('line version.\nestimated time: %f' % (finish - start))
        ax.set_xlabel('x')
        ax.set_ylabel('y')
        ax.set_zlabel('u')
        plt.savefig('1w.png')
    # python3 app.py
