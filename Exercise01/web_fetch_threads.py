#This program uses urllib to download web pages
import urllib.request as req
import time
import threading
import queue

hosts = ["http://missouri.edu/", "http://www.missouristate.edu/",
         "http://www.mssu.edu/", "http://www.semo.edu/",
         "http://umkc.edu", "http://mst.edu", "https://www.ucmo.edu/",
         "http://www.umsl.edu/", "http://www.truman.edu/",
         "http://www.nwmissouri.edu/"]


#get web pages from hosts and print first 50 characters
def get_page(que):
    #for host in hosts:
    host = que.get()
    url = req.urlopen(host)
    html = url.read()
    print(host, html[:50])
    que.task_done()

def main():
    #get_page(hosts)
    que = queue.Queue()
    for j in range(len(hosts)):
        que.put(hosts[j])
    for i in range(len(hosts)):
        t = threading.Thread(target = get_page, args=[que], name = "t"+str(i))
        t.setDaemon(True) #allows main prog to exit if threads are still running
        t.start()
    que.join()

start = time.time()
main()
print("Elapsed time: %s" % (time.time() - start))
