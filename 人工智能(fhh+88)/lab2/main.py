# 导入黑白棋文件
from game import Game  
from Random_player import RandomPlayer
from Human_player import HumanPlayer
from AI import AIPlayer

# 人类玩家黑棋初始化
black_player = RandomPlayer("X")

# 随机玩家白棋初始化
white_player = RandomPlayer("O")

# 游戏初始化，第一个玩家是黑棋，第二个玩家是白棋
# game = Game(black_player, white_player)

# # 开始下棋
# game.run()
x_success = 0
o_success = 0
draw = 0
for i in range(10000):
    game = Game(black_player, white_player)
    result ,diff = game.run()
    if result == 'black_win':
        x_success += 1
    elif result == 'white_win':
        o_success += 1
    else:
        draw += 1
    print("X: ", x_success / (i + 1) * 100, "% O: ", o_success / (i + 1) * 100, "% Draw: ", draw / (i + 1) * 100, "%")
print("FINAL X: ", x_success / (i + 1) * 100, "% O: ", o_success / (i + 1) * 100, "% Draw: ", draw / (i + 1) * 100, "%")


#测试
# 导入棋盘文件
# from board import Board
# board = Board()
# ai_player = AIPlayer("X")
# ai_player.get_move(board)