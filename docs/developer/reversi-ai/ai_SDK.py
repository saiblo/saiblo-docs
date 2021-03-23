'''
    ai_SDK for 黑白棋
'''

import sys
import ai_1

#pylint: disable = R0201
#pylint: disable = R0801

class logic_API:
    '''
        游戏逻辑提供的逻辑接口
    '''
    def get_id(self, x, y):
        '''
        获取(x,y)点对应的下标
        '''
        return (x << 3)+y

    def ask_next_pos(self, board, player):
        '''
        返回player在当前board下的可落子点
        '''
        ask_message = ['#', str(player)]
        for i in board:
            ask_message.append(str(i))
        ask_message.append('#')
        sys.stdout.buffer.write(ai_convert_byte("".join(ask_message)))
        sys.stdout.flush()
        data = sys.stdin.buffer.read(64)
        str_list = list(data.decode())
        return [int(i) for i in str_list]

def ai_convert_byte(data_str):
    '''
    传输数据的时候加数据长度作为数据头
    '''
    message_len = len(data_str)
    message = message_len.to_bytes(4, byteorder='big', signed=True)
    message += bytes(data_str, encoding="utf8")
    return message

def send_opt(data_str):
    '''
    发送自己的操作
    '''
    sys.stdout.buffer.write(ai_convert_byte(data_str))
    sys.stdout.flush()

def start():
    '''
    循环入口
    '''
    logic_message = logic_API()
    read_buffer = sys.stdin.buffer
    while True:
        data = read_buffer.read(67)
        now_player = int(data.decode()[1])
        str_list = list(data.decode()[2:-1])
        board_list = [int(i) for i in str_list]
        x, y = ai_1.player_ai(board_list, now_player, logic_message)
        send_opt(str(x)+str(y))

if __name__ == '__main__':
    start()
