import asyncio
from threading import Thread

class LoopShowerThread(Thread):
    def run(self):
        try:
            loop = asyncio.get_event_loop()
            print(loop)
        except RuntimeError:
            print("No event loop!")

loop = asyncio.get_event_loop()
print(loop)
thread = LoopShowerThread()
thread.start()
thread.join()
