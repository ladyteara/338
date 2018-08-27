#From Parallel Programming Cookbook, ch 3, p73
#Spawn a process
import multiprocessing as mp

def foo(i):
    print("Called function in process:", i, __name__)
    print(mp.current_process().pid, " - ", mp.current_process().name)
    return

def foo2(i):
    print("    Called foo2 in process:", i, __name__)
    print(mp.current_process().pid, " - ", mp.current_process().name)
    return

def main():
    Process_jobs = []
    print(__name__)
    for i in range(4):
        if i%2 is 1:
            p = mp.Process(target = foo, name = "odd" + str(i), args = [i])
            Process_jobs.append(p)
            p.start()
        if i%2 is 0:
            p = mp.Process(target = foo2, name= "even" + str(i), args = [i])
            Process_jobs.append(p)
            p.start()
        #p.join()

if __name__ == "__main__":  
    main()

