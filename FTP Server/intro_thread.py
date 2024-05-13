import threading
import time

# Creating a thread

def func1():
    for i in range(5):
        print("func1")
        time.sleep(1)

def func2():
    for i in range(5):
        print("func2")
        time.sleep(1)

def func3():
    for i in range(5):
        print("func3")
        time.sleep(1)


func1()
func2()
func3()

t1 = threading.Thread(target=func1)
t2 = threading.Thread(target=func2)
t3 = threading.Thread(target=func3)

t1.start()
t2.start()
t3.start()

# Use of join

t1.join()
t2.join()


print("program ended")

# Locking in threading

lock = threading.Lock()

x = 10

def increse():
    global x , lock
    lock.acquire()
    while x < 15:
        x += 2
        print(x)
        time.sleep(2)
    print("Maximum")
    lock.release()

def decrease():
    global x , lock
    lock.acquire()
    while x > 5:
        x -= 2
        print(x)
        time.sleep(2)
    print("Minimum")
    lock.release()

t1 = threading.Thread(target=increse).start()
t2 = threading.Thread(target=decrease).start()

# Semaphore

semaphor = threading.BoundedSemaphore(value=5)

def func(x):
    global semaphor

    print(f"{x} is accessing")
    semaphor.acquire()
    print(f"{x} accessed")
    time.sleep(5)
    semaphor.release()
    print(f"{x} is relaesing the thread")

for i in range(10):
    t = threading.Thread(target=func,args=(i,)).start()

# Use of Event

event = threading.Event()

def func():
    print("waiting for the event")
    event.wait()
    print("Event trigerred")

t = threading.Thread(target=func).start()

input("Enter to trigger")
event.set()

# Daemon thread

text = ""

def read_file():
    global text
    while True:
        with open("text.txt","r") as f:
            text = f.readline()
            time.sleep(3)

def print_file():
    global text
    for i in range(30):
        print(text)
        time.sleep(1)

t = threading.Thread(target=read_file,daemon=True)
t2 = threading.Thread(target=print_file)

t.start()
t2.start()


