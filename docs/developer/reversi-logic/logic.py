'''
黑白棋游戏逻辑
'''

import sys
import json
import logic_python_SDK

#pylint: disable = R1702
#pylint: disable = R0801

DEBUG = True

dx = [0, 0, 1, 1, 1, -1, -1, -1]
dy = [1, -1, 1, 0, -1, 1, 0, -1]

class Game:
    '''
    游戏类
    '''
    def __init__(self):
        '''
        初始化
        '''
        self.board = [2]*64
        self.board[get_id(3, 3)] = 1
        self.board[get_id(4, 4)] = 1
        self.board[get_id(4, 3)] = 0
        self.board[get_id(3, 4)] = 0
        self.state = 1
        self.listen = 0
        self.replay_list = []
        self.player_type = []
        self.replay_file = None

    def get_game_end(self):
        '''
        获取游戏终局信息并结束对局
        '''
        first_cnt = 0
        second_cnt = 0
        for i in self.board:
            if i == 1:
                second_cnt += 1
            elif i == 0:
                first_cnt += 1
        if first_cnt > second_cnt:
            self.game_end(1, 0)
        elif first_cnt < second_cnt:
            self.game_end(0, 1)
        else:
            self.game_end(0, 0)

    def get_next_player(self):
        '''
        切换到下一个玩家
        '''
        self.listen = 1-self.listen
        can_flag = False
        for x in range(0, 8):
            for y in range(0, 8):
                if board_check(self.board, x, y, self.listen):
                    can_flag = True
                    break
            if can_flag:
                break
        if not can_flag:
            self.listen = 1-self.listen
            for x in range(0, 8):
                for y in range(0, 8):
                    if board_check(self.board, x, y, self.listen):
                        can_flag = True
                        break
                if can_flag:
                    break
            if not can_flag:
                self.get_game_end()

    def get_state_opt(self):
        '''
        监听循环
        '''
        while True:
            opt_dict = logic_python_SDK.read_opt()
            if opt_dict['player'] == self.listen:
                opt = opt_dict['content']
                if self.player_type[self.listen] == 2:
                    opt = json.loads(opt)
                    if opt['state'] != self.state:
                        continue
                    else:
                        opt = opt['opt']
                #查询可走位置的接口
                if opt[0] == '#':
                    now_player = int(opt[1])
                    board_str = list(opt[2:-1])
                    board_list = [int(i) for i in board_str]
                    ans_list = []
                    for x in range(0, 8):
                        for y in range(0, 8):
                            if board_check(board_list, x, y, now_player):
                                ans_list.append("1")
                            else:
                                ans_list.append("0")
                    logic_python_SDK.send_message_goal("".join(ans_list), self.listen)

                else:
                    x, y = get_opt(opt)
                    self.board_change(x, y, self.listen)
                    self.get_next_player()
                    self.state += 1
                    break
            elif opt_dict['player'] == -1:
                opt = json.loads(opt_dict['content'])
                if opt['error'] == 0:
                    if opt['player'] == 0:
                        self.game_end(0, 1)
                    else:
                        self.game_end(1, 0)
                else:
                    if self.listen == 0:
                        self.game_end(0, 1)
                    else:
                        self.game_end(1, 0)

    def board_change(self, x, y, player):
        '''
        更改局面信息
        '''
        if not board_check(self.board, x, y, player):
            if player == 0:
                self.game_end(0, 1)
            else:
                self.game_end(1, 0)
        else:
            self.board[get_id(x, y)] = player
            for index in range(0, 8):
                nx = x
                ny = y
                flag = False
                while True:
                    nx = nx+dx[index]
                    ny = ny+dy[index]
                    if not get_check(nx, ny):
                        break
                    now_id = get_id(nx, ny)
                    if self.board[now_id] == 2:
                        break
                    if self.board[now_id] == player:
                        if flag:
                            self.reverse(x, y, nx, ny, index)
                        break
                    if self.board[now_id] == 1-player:
                        flag = True

    def reverse(self, x, y, nx, ny, index):
        '''
        翻转棋子
        '''
        tx = x+dx[index]
        ty = y+dy[index]
        while tx != nx or ty != ny:
            now_id = get_id(tx, ty)
            self.board[now_id] = 1-self.board[now_id]
            tx = tx+dx[index]
            ty = ty+dy[index]

    def get_state_dict(self, player_list, content_list):
        '''
        获取局面描述的字典
        '''
        state_dict = {}
        state_dict['state'] = self.state
        state_dict['listen'] = [self.listen]
        state_dict['player'] = player_list
        state_dict['content'] = content_list
        return state_dict

    def board_message(self):
        '''
        返回局面描述的字符串
        '''
        player_list = []
        content_list = []
        for index, type in enumerate(self.player_type):
            if type == 2:
                player_list.append(index)
                player_dict = self.replay_state_dict()
                player_dict['my_color'] = index
                player_dict['now_player'] = self.listen
                content_list.append(json.dumps(player_dict))
            elif index == self.listen:
                player_list.append(index)
                board_str = [str(self.listen)]
                for num in self.board:
                    board_str.append(str(num))
                content_list.append("#"+"".join(board_str)+"#")
        return player_list, content_list

    def replay_state_dict(self):
        state_dict = {}
        state_dict['state'] = self.state
        state_dict['maze'] = "".join([str(num) for num in self.board])
        return state_dict

    def start(self):
        '''
        开始游戏
        '''
        self.game_init(logic_python_SDK.read_player_state())
        while True:
            self.replay_list.append(self.replay_state_dict())
            player_list, content_list = self.board_message()
            logic_python_SDK.send_state(self.get_state_dict(player_list, content_list))
            self.get_state_opt()

    def game_init(self, init_dict):
        '''
        游戏初始化处理
        '''
        self.replay_file = open(init_dict['replay'], 'w')
        player_list = init_dict['player_list']
        self.player_type = player_list
        first_state = player_list[0]
        second_state = player_list[1]
        if first_state == 0 or second_state == 0:
            self.game_end(first_state, second_state)

    def game_end(self, first_state, second_state):
        '''
        游戏终局处理
        '''
        first_score = 0
        second_score = 0
        if first_state == second_state:
            first_score = 1
            second_score = 1
        elif first_state == 0:
            first_score = 0
            second_score = 1
        else:
            first_score = 1
            second_score = 0
        dict_end = {"0" : first_score, "1": second_score, "state": -1}
        dict_end['maze'] = "".join([str(num) for num in self.board])
        player_list = []
        content_list = []
        for index, type in enumerate(self.player_type):
            if type == 2:
                player_list.append(index)
                content_list.append(json.dumps(dict_end))
        logic_python_SDK.send_state(self.get_state_dict(player_list, content_list))
        self.replay_list.append(dict_end)
        self.replay_file.write(json.dumps(self.replay_list))
        self.replay_file.close()
        dict_end.pop('state')
        dict_end.pop('maze')
        logic_python_SDK.send_end_info(json.dumps(dict_end))
        sys.exit()

def get_opt(opt_str):
    '''
    获取操作
    '''
    x = int(opt_str[0])
    y = int(opt_str[1])
    return x, y

def get_check(x, y):
    '''
    检查位置是否合法
    '''
    if x < 0 or x > 7:
        return False
    if y < 0 or y > 7:
        return False
    return True

def get_id(x, y):
    '''
    获取(x,y)转换下标
    '''
    return (x << 3)+y

def board_check(board, x, y, player):
    '''
    检查位置是否可走
    '''
    if not get_check(x, y):
        return False
    now_id = get_id(x, y)
    if board[now_id] != 2:
        return False
    for index in range(0, 8):
        nx = x
        ny = y
        flag = False
        while True:
            nx = nx+dx[index]
            ny = ny+dy[index]
            if not get_check(nx, ny):
                break
            now_id = get_id(nx, ny)
            if board[now_id] == 2:
                break
            if board[now_id] == player:
                if flag:
                    return True
                break
            if board[now_id] == 1-player:
                flag = True
    return False

def main():
    '''
    主函数
    '''
    game = Game()
    game.start()

if __name__ == '__main__':
    main()
