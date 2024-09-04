import numpy as np           # 提供维度数组与矩阵运算
import copy                  # 从copy模块导入深度拷贝方法
from board import Chessboard

# 基于棋盘类，设计搜索策略
class Game:
    def __init__(self, show = True):
        """
        初始化游戏状态.
        """
        #初始化棋盘
        self.chessBoard = Chessboard(show)
        #初始化结果位次
        self.solves = []
        self.gameInit()
        
    # 重置游戏
    def gameInit(self, show = True):
        """
        重置棋盘.
        """
        
        self.Queen_setRow = [-1] * 8
        self.chessBoard.boardInit(False)
        
    ##############################################################################
    ####                请在以下区域中作答(可自由添加自定义函数)                 #### 
    ####              输出：self.solves = 八皇后所有序列解的list                ####
    ####             如:[[0,6,4,7,1,3,5,2],]代表八皇后的一个解为                ####
    ####           (0,0),(1,6),(2,4),(3,7),(4,1),(5,3),(6,5),(7,2)            ####
    ##############################################################################
    #                                                                            #
    
    def run(self, row=0):
        #self.solves.append([0,6,4,7,1,3,5,2])
        #每次开始前初始化（需要确定轮次，可以以第一行为计算）
        count = 0
        number = 0
        while count < 8:
            self.gameInit(False)
            self.chessBoard.setQueen(0,count,False)
            self.check_level(number + 1)
            count += 1
            
        
    def check_level(self, number):
        #需要拷贝一份
        chessboard_copy = copy.deepcopy(self.chessBoard)
        #进行棋子摆放的check
        if number == 8:
            #记录正确的结果
            #print('---find a result---')
            #self.chessBoard.printChessboard()
            solve_list = []
            for i in range(8):
                for j in range(8):
                    if self.chessBoard.queenMatrix[i][j] == 1:
                        solve_list.append(j)
                        break
            #print(solve_list)
            self.solves.append(solve_list)
        else:
            for i in range(8):
                if self.chessBoard.setQueen(number, i, False):
                    self.check_level(number + 1)
                    self.chessBoard = copy.deepcopy(chessboard_copy)


    #                                                                            #
    ##############################################################################
    #################             完成后请记得提交作业             ################# 
    ##############################################################################
    
    def showResults(self, result):
        """
        结果展示.
        """
        
        self.chessBoard.boardInit(False)
        for i,item in enumerate(result):
            if item >= 0:
                self.chessBoard.setQueen(i,item,False)
        
        self.chessBoard.printChessboard(False)
    
    def get_results(self):
        """
        输出结果(请勿修改此函数).
        return: 八皇后的序列解的list.
        """
        
        self.run()
        return self.solves
   

game = Game(False)
solutions = game.get_results()
#print('There are {} results.'.format(len(solutions)))
#game.showResults(solutions[0])
#print(solutions)
print(len(solutions))