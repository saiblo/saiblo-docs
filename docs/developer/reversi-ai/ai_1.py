'''
用户需要编写的AI函数
'''

'''
    ai_SDK for 黑白棋
'''

import sys

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

def player_ai(board, player, logic):
    '''
    用户需要编写的AI函数
    参数:board 长度为64的list, 0表示player0的子, 1表示player1的子, 2表示没有落子
        player 为0或1,表示当前该谁落子
        logic  调用游戏逻辑的API
               目前支持的操作有:
                    get_id(x, y):获取(x,y)点在长度为64的board list里的下标
                    ask_next_pos(board, player): 参数为长度为64描述棋盘的list board和描述当前落子方的player
                                                 返回值为长度为64的list, list中数字1代表可以落子, 数字0代表不可落子
    '''
    while True:
        pass
    next_list = logic.ask_next_pos(board, player)
    for x in range(0, 8):
        for y in range(0, 8):
            if next_list[logic.get_id(x, y)] == 1:
                return x, y
    return 0, 0


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
        x, y = player_ai(board_list, now_player, logic_message)
        send_opt(str(x)+str(y))

if __name__ == '__main__':
    start()
