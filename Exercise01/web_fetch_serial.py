#This program uses urllib to download web pages
import urllib.request as req
import time

hosts = ["http://missouri.edu/", "http://www.missouristate.edu/",
         "http://www.mssu.edu/", "http://www.semo.edu/",
         "http://umkc.edu", "http://mst.edu", "https://www.ucmo.edu/",
         "http://www.umsl.edu/", "http://www.truman.edu/",
         "http://www.nwmissouri.edu/"]

#get web pages from hosts and print first 50 characters
def get_page(hosts):
    for host in hosts:
        url = req.urlopen(host)
        html = url.read()
        print(host, html[:50])

def main():
    get_page(hosts)

start = time.time()
main()
print("Elapsed time: %s" % (time.time() - start))
