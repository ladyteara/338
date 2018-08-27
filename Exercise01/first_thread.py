import threading
import time

def fun1(x, y):
    """Executes inside a thread"""
    print("\tThread name : ", threading.currentThread().getName())
    print("\t" + str(x), y)
    print("\tHello from a thread")
    time.sleep(1)
    print("\tGoodbye from a thread")
    return

def fun2 ():
    print("Hello from", threading.currentThread().getName())
    time.sleep(5)
    print("G'bye from", threading.currentThread().getName())
    return

def fun3():
    print("Hello from", threading.currentThread().getName())
    time.sleep(3)
    print("G'bye from", threading.currentThread().getName())
    return

if __name__ == "__main__":
    print("Program name: ", __name__)
    print("Thread name : ", threading.currentThread().getName())
    #print()
    t1 = threading.Thread(target = fun1, args =[42, "yellow"], name = "fun1") #Create a thread object
    t2 = threading.Thread(target = fun2, name = "fun2")
    t3 = threading.Thread(target = fun3, name = "fun3")
    t1.start()  #Start the thread
    t1.join()   #Clean up after the thread; blocks until thread completes
    t2.start()  #Start the thread
    t2.join()
    t3.start()  #Start the thread
    t3.join()
