'''
裁判程序
'''

import sys
import threading
import shlex
import subprocess
import json
import time

#pylint: disable = W0702

DEBUG = True

class Player(threading.Thread):
    '''
    监听ai发送的消息的线程
    '''
    def __init__(self, judge_thread, command, player_id):
        '''
        初始化,启动ai进程
        '''
        threading.Thread.__init__(self)
        self.judge_thread = judge_thread
        self.end_tag = False
        self.player_id = player_id
        self.length_limit = 2048
        #尝试启动AI进程并检测异常
        try:
            self.subpro = subprocess.Popen(shlex.split(command), stdout=subprocess.PIPE,\
                stdin=subprocess.PIPE, universal_newlines=True)
        except:
            self.end_tag = True
            #self.judge_thread.send_start_error(player_id, command)

    def change_length(self, length):
        '''
        变更长度
        '''
        self.length_limit = length

    def destory(self):
        '''
        退出run函数循环
        '''
        self.end_tag = True
        try:
            self.subpro.terminate()
        except:
            pass
        self.judge_thread.awake()

    def write(self, msg):
        '''
        向ai发送消息
        '''
        try:
            self.subpro.stdin.buffer.write(msg)
            self.subpro.stdin.flush()
        except:
            pass

    def run(self):
        '''
        监听ai发送的消息
        '''
        while not self.end_tag:
            return_code = self.subpro.poll()
            if return_code is not None:
                self.judge_thread.send_run_error(self.player_id)
                break
            try:
                read_buffer = self.subpro.stdout.buffer
                data_len = int.from_bytes(read_buffer.read(4), byteorder='big', signed=True)
                if data_len > self.length_limit:
                    data_len = 0
                else:
                    data = read_buffer.read(data_len)
            except:
                self.judge_thread.send_run_error(self.player_id)
                break
            else:
                if data_len != 0:
                    self.judge_thread.receive_message(data.decode(), self.player_id)

class Judger(threading.Thread):
    '''
    和逻辑及控制器通信的线程
    '''
    def __init__(self, command):
        '''
        线程初始化
        '''
        threading.Thread.__init__(self)
        self.mutex_num = threading.Lock()
        self.mutex_buffer = threading.Lock()
        self.mutex_listen = threading.Lock()
        self.event = threading.Event()
        self.time_limit = 4
        self.length_limit = 2048
        self.listen_list = [] #监听列表
        self.active_list = [] #存活AI列表
        self.player_list = [] #玩家列表
        self.player_num = 0
        self.end_tag = False
        self.time_thread = Time_thread(self, self.time_limit)
        self.time_thread.start()
        #尝试启动逻辑进程并检测异常
        try:
            self.subpro = subprocess.Popen(shlex.split(command), stdout=subprocess.PIPE,\
                stdin=subprocess.PIPE, universal_newlines=True)
        except:
            logic_start_error(command)

    def awake(self):
        '''
        确保所有ai进程退出后唤醒线程发送消息
        '''
        self.mutex_num.acquire()
        self.player_num = self.player_num-1
        if self.player_num == 0:
            self.event.set()
        self.mutex_num.release()

    def init(self, player_list):
        '''
        初始化
        '''
        self.player_list = player_list
        self.player_num = len(player_list)
        for player_thread in self.player_list:
            if player_thread.end_tag:
                self.active_list.append(0)
            else:
                self.active_list.append(1)
        for player_thread in self.player_list:
            player_thread.change_length(self.length_limit)
        self.send_start_error(self.active_list)


    def clear_all(self):
        '''
        清理线程
        '''
        self.event.clear()
        for player_thread in self.player_list:
            player_thread.destory()
        try:
            self.subpro.terminate()
        except:
            pass
        self.time_thread.awake(-1)
        self.time_thread.join()
        self.event.wait(3)

    def write(self, msg):
        '''
        向逻辑发送消息
        '''
        self.mutex_buffer.acquire()
        try:
            self.subpro.stdin.buffer.write(msg)
            self.subpro.stdin.flush()
        except:
            pass
        self.mutex_buffer.release()

    def receive_message(self, data, player_id):
        '''
        接收选手消息后的处理(看效率可能会修改成队列实现)
        '''
        send_flag = False
        self.mutex_listen.acquire()
        for listen_id in self.listen_list:
            if listen_id == player_id:
                send_flag = True
                break
        self.mutex_listen.release()
        if send_flag:
            player_message_dict = {'player': player_id, 'content': data}
            self.write(convert_byte(json.dumps(player_message_dict)))

    def send_message(self, logic_data):
        '''
        解析json并发送消息
        '''
        #bug待修复:缺少对json数据的类型检查
        try:
            data = json.loads(logic_data)
            game_state = data['state']
        except:
            self.clear_all()
            logic_decode_error(logic_data.decode())
            return True
        if not isinstance(game_state, int):
            self.clear_all()
            logic_decode_error(logic_data.decode())
            return True
        if game_state > 0:
            self.mutex_listen.acquire()
            try:
                self.listen_list = data['listen']
                send_list = data['player']
                data_list = data['content']
            except:
                self.clear_all()
                logic_decode_error(logic_data.decode())
                return True
            self.mutex_listen.release()
            for player_index, content in enumerate(data_list):
                try:
                    self.player_list[send_list[player_index]].write(\
                        bytes(content, encoding="utf-8"))
                except:
                    self.clear_all()
                    logic_send_error(logic_data.decode())
                    return True
            self.time_thread.awake(game_state)
        elif game_state < 0:
            self.clear_all()
            try:
                end_str = data['end_info']
            except:
                logic_decode_error(logic_data.decode())
            else:
                sys.stdout.buffer.write(convert_byte(end_str))
        else:
            if data.__contains__('time'):
                self.time_limit = data['time']
                self.time_thread.change(self.time_limit)
            if data.__contains__('length'):
                self.length_limit = data['length']
                for player_thread in self.player_list:
                    player_thread.change_length(self.length_limit)
        return game_state < 0

    def send_start_error(self, active_list):
        '''
        向逻辑发送启动异常信息
        '''
        error_data = {"player_list": active_list}
        self.write(convert_byte(json.dumps(error_data)))

    def send_run_error(self, player_id):
        '''
        向逻辑发送运行异常信息
        '''
        error_content = {'player': player_id, 'error': 0, 'error_log': "RUN ERROR"}
        error_data = {'player': -1, 'content': json.dumps(error_content)}
        self.write(convert_byte(json.dumps(error_data)))

    def send_timeout_error(self, state):
        '''
        向逻辑发送超时信息
        '''
        error_content = {'player': state, 'error': 1, 'error_log': "TIMEOUT ERROR"}
        error_data = {'player': -1, 'content': json.dumps(error_content)}
        self.write(convert_byte(json.dumps(error_data)))

    def run(self):
        '''
        监听逻辑发送的消息
        '''
        while True:
            return_code = self.subpro.poll()
            try:
                read_buffer = self.subpro.stdout.buffer
                data_len = int.from_bytes(read_buffer.read(4), byteorder='big', signed=True)
                data = read_buffer.read(data_len) #这里会出现缓冲区大小不够的问题，可能需要进一步修改
            except:
                #逻辑进程崩溃
                self.clear_all()
                logic_run_error()
                break
            else:
                if data_len != 0:
                    end_tag = self.send_message(data)
                    if end_tag:
                        break
            if return_code is not None:
                self.clear_all()
                logic_run_error()
                break

class Time_thread(threading.Thread):
    '''
    用来计时的线程
    '''
    def __init__(self, judger, time_limit):
        '''
        初始化
        '''
        threading.Thread.__init__(self)
        self.time_limit = time_limit
        self.start_time = time.time()
        self.judger = judger
        self.state = 0
        self.now_state = 0
        self.event = threading.Event()
        self.mutex = threading.Lock()
        self.event.clear()

    def change(self, time_limit):
        '''
        修改时间限制
        '''
        self.mutex.acquire()
        self.time_limit = time_limit
        self.mutex.release()

    def awake(self, state):
        '''
        唤醒线程
        '''
        self.mutex.acquire()
        self.state = state
        self.event.set()
        self.mutex.release()

    def run(self):
        while True:
            self.event.wait()
            self.mutex.acquire()
            self.now_state = self.state
            if self.state == -1:
                self.mutex.release()
                break
            self.mutex.release()
            #开始计时
            self.start_time = time.time()
            while True:
                self.mutex.acquire()
                if self.now_state == self.state:
                    end_time = time.time()
                    if end_time-self.start_time >= self.time_limit:
                        self.judger.send_timeout_error(self.now_state)
                        self.mutex.release()
                        break
                else:
                    self.mutex.release()
                    break
                self.mutex.release()
            self.mutex.acquire()
            if self.now_state == self.state:
                self.event.clear()
            self.mutex.release()


def system_convert():
    '''
    获取命令行参数
    '''
    logic_command = sys.argv[1].replace("+", " ")
    logic_argc = int(sys.argv[2])
    if logic_argc > 0:
        logic_command = logic_command+" "+' '.join(sys.argv[3:3+logic_argc])
    return logic_command, [command_str.replace("+", " ") for command_str in sys.argv[3+logic_argc:]]

def convert_byte(data_str):
    '''
    judger 传输数据的时候加数据长度作为数据头
    '''
    message_len = len(data_str)
    message = message_len.to_bytes(4, byteorder='big', signed=True)
    message += bytes(data_str, encoding="utf8")
    return message

def logic_start_error(error_str):
    '''
    处理逻辑进程启动失败异常
    '''
    #与控制器通信协议待定
    if DEBUG:
        sys.stdout.buffer.write(convert_byte("command:"+error_str+"\n"+"error:START FAILURE!"+"\n"))
    sys.exit()

def logic_run_error():
    '''
    处理逻辑进程崩溃异常
    '''
    #与控制器通信协议待定
    if DEBUG:
        sys.stdout.buffer.write(convert_byte("error: RUN ERROR!\n"))
        sys.stdout.flush()

def logic_decode_error(error_str):
    '''
    处理逻辑进程JSON格式错误异常
    '''
    #与控制器通信协议待定
    if DEBUG:
        sys.stdout.buffer.write(convert_byte(error_str+" DECODE ERROR!"))
        sys.stdout.flush()

def logic_send_error(error_str):
    '''
    处理逻辑进程发送数据异常
    '''
    #与控制器通信协议待定
    if DEBUG:
        sys.stdout.buffer.write(convert_byte(error_str+" SEND ERROR!"))
        sys.stdout.flush()

def main():
    '''
    主函数
    '''
    logic, path_list = system_convert()
    judge_thread = Judger(logic)
    player_thread_list = []
    for index, path in enumerate(path_list):
        player_thread_list.append(Player(judge_thread, path, index))
    for player_thread in player_thread_list:
        player_thread.start()
    judge_thread.init(player_thread_list)
    judge_thread.start()
    judge_thread.join()
    for player_thread in player_thread_list:
        player_thread.join()

if __name__ == "__main__":
    main()
