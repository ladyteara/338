#Generates random strings in processes and sends them to main() using a queue
import multiprocessing as mp
import random
import string

#Define an output queue
output = mp.Queue()

#Example function
def rand_string(length, output, x):
    """Generates a random string of numbers, lower/upper case characters"""
    rand_str = ''.join(random.choice(string.ascii_lowercase +
                                     string.ascii_uppercase +
                                     string.digits) for i in range(length))
    tpl = (x, rand_str)
    output.put(tpl)

def main():
    #set up a list of processes
    processes = [mp.Process(target=rand_string, args=[5, output, x]) for x in range(4)]
    #Run processes
    for p in processes:
        p.start()
    #Exit the completed processes
    for p in processes:
        p.join()
    #Get process results from the output queue
    results = [output.get() for p in processes]
    results.sort()
    print(results)

if __name__ == "__main__":
    main()
