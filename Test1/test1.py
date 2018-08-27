'''
Tara Walton     tara1984     Test 1
Calculate a global sum using multiple processes
'''
import sys
import multiprocessing as mp

def readfile(fname):
    '''Reads a text file; returns a list of integers'''
    f = open(fname, 'r')
    contents = f.read()
    numbers = contents.split()
    for i in range(len(numbers)):
        numbers[i] = int(numbers[i])
    #total = sum(numbers)
    #print("Total: ", total) #for check
    return numbers

def subtotal(numbers, q):
    '''Add the correct arguments to calculate the total of a list of numbers'''
    sub = sum(numbers)
    q.put(sub)
    return sub              #return the total

def main(num_processes):
    fname = "randnums.txt"
    numlist = readfile(fname)  #numlist is a list of integers
    #Your code here to determine which numbers from numlist to send
    #to each process, start the processes, get the subtotals, calculate
    #the global sum, and print the global sum
    
    que = mp.Queue()        #a place to store subtotals
    length = len(numlist)   #how many numbers to sum
    pCount = mp.cpu_count() #number of processors on the computer in use
    procs = []              #to accumulate processes for joining
    
    perSub = length//pCount #amt of numbers in an even split
    remNum = length%pCount  #remaining numbers after even split
    
    x=0
    #print(numlist[x:x+perSub]) #slice of list test
          
    for i in range(0, num_processes-1):   #n processes
        p = mp.Process(target=subtotal, args=[numlist[x:x+perSub], que])
        procs.append(p)
        x+=perSub

    px = mp.Process(target=subtotal, args=[numlist[x:x+perSub+remNum], que])
    procs.append(px)
    for p in procs:
        p.start()

    for p in procs:
        p.join()

    result = [que.get() for p in procs]
    sumTot = sum(result)

    print("Grand Total:", sumTot)
        
    

if __name__ == "__main__":
    #Your code here
    #   1. Get number of processes from the command line
    num = int(sys.argv[1])
    #   2. Call main() with the number of processes as an argument
    main(num)
    print("Program Complete")

