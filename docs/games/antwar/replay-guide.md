Saiblo网站的对局支持下载回放文件，回放文件格式为json，文件为一个json数组，每一个元素代表一个回合信息：

 ```json
 [
 	{
         "op0": [ // player0 操作数组
             { //
                 "args": -1, // 操作参数，没有设为-1
                 "id": -1, // 目标id参数
                 "pos": { // 位置参数
                     "x": 6,
                     "y": 4
                 },
                 "type": 11 // 操作类型
             }
         ],
         "op1": [ //player1 操作
         ],
         "round_state": { // 回合信息
             "anthpLv": [ // 大本营高级装甲升级
                 0,
                 0
             ],
             "ants": [ // 所有蚂蚁信息列表
                 {
                     "age": 0, //
                     "hp": 10,
                     "id": 0,
                     "level": 0,
                     "move": -1,
                     "player": 0,
                     "pos": {
                         "x": 2,
                         "y": 9
                     },
                     "status": 0
                 }
             ],
             "camps": [ // 大本营血量
                 50,
                 50
             ],
             "coins": [ //
                 6,
                 6
             ],
             "error": "", // 结束错误信息，仅对局异常结束时返回该字段，若出现异常情况会显示具体错误信息
             "message": "[,]", // 结束信息，为字符串类型，表示玩家结束时状态
             "pheromone": [], // 信息素数组，大小为[2][MAP_SIZE][MAP_SIZE]
             "speedLv": [ // 大本营生产流水线等级
                 0,
                 0
             ],
             "towers": [ // 防御塔增量信息，即只包含回合内新建/等级状态发生变化的塔
                 {
                     "cd": 1, // 防御塔cd
                     "id": 0, // 防御塔cd
                     "player": 0, // 所属玩家
                     "pos": { // 防御塔坐标
                         "x": 6,
                         "y": 4
                     },
                     "type": 0 // 防御塔类型
                 },
             ],
             "winner": -1 // 游戏结果，表示为玩家id，未结束时为-1
         }
     }
 ]
 ```

