import sys
import time
import logging
import threading
import websocket


remote_ai_ws = 'ws://127.0.0.1:7971'
client = websocket.WebSocketApp
error_msg = ""
quit_flag = False
connected_flag = False

MAX_DISCONNECT_COUNT = 5
DEBUG_MODE = True
time_stamp = time.strftime('%Y-%m-%d-%H-%M-%S', time.localtime(time.time()))
# logging.basicConfig(filename='local_log_' + time_stamp + ".txt", level=logging.INFO)
logging.basicConfig(filename='local_log.txt', level=logging.INFO)
logging.info('start@  %s' % str(time_stamp))

def gprint(msg):
    if DEBUG_MODE:
        logging.info(" %s" % str(msg))


def convert_byte(data_str):
    '''
    传输数据的时候加数据长度作为数据头
    '''
    message_len = len(data_str)
    message = message_len.to_bytes(4, byteorder='big', signed=True)
    message += bytes(data_str, encoding="utf8")
    return message


def send_state(msg):
    '''
    发送回合信息
    '''
    sys.stdout.buffer.write(convert_byte(msg))
    sys.stdout.flush()


def on_open(ws):
    gprint('connected to remote AI as ' + str(ws))
    global connected_flag
    connected_flag = True


def on_message(ws, msg):
    gprint('send to judger: ' + str(msg))
    print(msg)


def on_close(ws):
    gprint('disconnect from remote AI')


def on_error(ws, err):
    global error_msg
    error_msg = err


def maintain_websocket():
    global client, quit_flag
    disconnect_count = 0
    while disconnect_count < MAX_DISCONNECT_COUNT:
        client = websocket.WebSocketApp(remote_ai_ws,
                                        on_message=on_message,
                                        on_error=on_error,
                                        on_close=on_close,
                                        on_open=on_open)
        # websocket.enableTrace(True)
        client.run_forever()
        disconnect_count += 1
    gprint('failed to connect to remote ai.')
    quit_flag = True


def main():
    ws_thread = threading.Thread(target=maintain_websocket)
    ws_thread.start()
    while not connected_flag:
        continue
    while not quit_flag:
        read_buffer = sys.stdin.buffer
        try:
            gprint('blocking in reading from judger ...')
            data = read_buffer.read(67)
            # data_len = int.from_bytes(read_buffer.read(67), byteorder='big', signed=True)
            # data = read_buffer.read(data_len)
            gprint('read from judger ' + str(data))
            if not quit_flag:
                client.send(data)
        except:
            logging.exception('Error occured when read from judger.')


if __name__ == '__main__':
    try:
        main()
    except:
        logging.exception('Unknown error:')
    finally:
        logging.info('programme exit')
