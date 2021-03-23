import os
import time
import shlex
import logging
import threading
import subprocess
import websockets
import asyncio
from tkinter import *
import tkinter.filedialog

IP_ADDRESS = '127.0.0.1'
PORT = 7971

# logging.basicConfig(filename='remote_log.txt', level=logging.INFO)
logging.basicConfig(level=logging.INFO)

exe_file = ''
main_loop = asyncio.get_event_loop()

DEBUG_MODE = True


def query(msg: str) -> bool:
    print(msg, ' [Y/N]')
    s = input()
    if s[0] == 'Y':
        return True
    else:
        return False


def query_get(msg: str) -> str:
    print(msg, ' input here:')
    return input()


class Ai(threading.Thread):

    def __init__(self, ws):
        threading.Thread.__init__(self)
        self.websocket = ws
        self.end_flag = False
        try:
            self.subpro = subprocess.Popen(shlex.split(exe_file), stdout=subprocess.PIPE,
                                           stdin=subprocess.PIPE, universal_newlines=True)
        except Exception:
            logging.exception('Error occured when opening file: %s' % exe_file)
        else:
            logging.info('Successfully open subprocess:\{ %s \}' % exe_file)

    def run(self) -> None:
        while not self.end_flag:
            try:
                read_buffer = self.subpro.stdout.buffer
                bytes_len = read_buffer.read(4)
                msg_len = int.from_bytes(bytes_len, byteorder='big', signed=True)
                msg = read_buffer.read(msg_len)
                logging.info('ready to send to %s with message %s\n' % (self.websocket, str(bytes_len + msg)))
                server.send_msg(self.websocket, bytes_len + msg)
                logging.info('send to %s with message %s\n' % (self.websocket, str(bytes_len + msg)))
            except Exception:
                logging.exception('Error occured when receiving message from AI.')

    def write(self, msg):
        try:
            send_msg = msg
            if type(send_msg) == type(''):
                send_msg = bytes(send_msg, encoding='utf-8')
            logging.info('writing: %s' % send_msg)
            self.subpro.stdin.buffer.write(send_msg)
            self.subpro.stdin.flush()
            logging.info('finish write.')
        except Exception:
            logging.exception('Error occured when writing message to AI.')
        else:
            if DEBUG_MODE:
                logging.info('read from %s with message %s' % (self.websocket, msg))


class RemoteAiServer(threading.Thread):

    def __init__(self):
        threading.Thread.__init__(self)
        self.connected = dict()

    async def handler(self, websocket, path):
        logging.info('Websocket connected.')
        ai = Ai(websocket)
        ai.start()
        self.connected[websocket] = ai
        try:
            async for msg in websocket:
                ai.write(msg)
        finally:
            self.connected.pop(websocket)

    def send_msg(self, websocket, msg):
        logging.info('send to remote AI %s' % msg)
        coro = websocket.send(msg)
        future = asyncio.run_coroutine_threadsafe(coro, main_loop)


server = RemoteAiServer()


def main():
    global exe_file, main_loop
    root = Tk()
    root.withdraw()
    filename = tkinter.filedialog.askopenfilename()
    logging.info('Selected file: %s' % filename)
    if not os.path.exists(filename):
        logging.error('No such file: %s' % filename)
        quit()
    tmp, extension = os.path.splitext(filename)
    if extension != '':
        flag = query('Is this an executable file?')
        if not flag:
            msg = query_get('What is the interpreter of this file?')
            exe_file = msg + ' '
        else:
            exe_file = ''
    exe_file += filename + ' remote'

    ws_server = websockets.serve(server.handler, IP_ADDRESS, PORT)
    main_loop.run_until_complete(ws_server)
    main_loop.run_forever()


if __name__ == '__main__':
    logging.info('Start@    ' + time.strftime('%Y/%m/%d  %H:%M:%S', time.localtime(time.time())))
    main()
