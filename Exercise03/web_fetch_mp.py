#This program uses urllib to download web pages
import urllib.request as req
import time
import multiprocessing as mp
import threading
import queue

hosts = ["http://missouri.edu/", "http://www.missouristate.edu/",
         "http://www.mssu.edu/", "http://www.semo.edu/",
         "http://umkc.edu", "http://mst.edu", "https://www.ucmo.edu/",
         "http://www.umsl.edu/", "http://www.truman.edu/",
         "http://www.nwmissouri.edu/"]


#get web pages from hosts and print first 50 characters
def get_page(pages):
    for pg in pages:
        url = req.urlopen(pg)
        html = url.read()
        print(mp.current_process().name, pg, html[:50])
        

def main():
    #spawn processes
    procs = [] #to accumulate the processes for joining
    for i in range(0, len(hosts), 2):
        p = mp.Process(target = get_page, args=[[hosts[i], hosts[i+1]]], name = "p"+str(i))
        procs.append(p)
        p.start()
    for p in procs:
        p.join()


start = time.time()
if __name__ == "__main__":
    main()
    print("Elapsed time: %s" % (time.time() - start))
