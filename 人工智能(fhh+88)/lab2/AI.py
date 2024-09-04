from copy import deepcopy
from func_timeout import func_timeout, FunctionTimedOut
import math
import random
MY_CONSTANT = 999999999
TOTAL_pos = 64
ROUND = 1
FACTOR = 10

class TreeNode:
    def __init__(self, score = 0, visits = 0, action = None, parent = None, color = None):
        self.score = score
        self.visits = visits
        self.action = action
        self.parent = parent
        self.ucb = MY_CONSTANT
        self.color = color
        self.children = {}

    #输出树形结构
    def print_tree(self, depth = 0):
        if self.ucb == -MY_CONSTANT or ( self.ucb != MY_CONSTANT and 1):#self.parent.ucb == -MY_CONSTANT ):
            print("  " * depth + f"Action: {self.action}, Color: {self.color}, Score: {self.score}, Visits: {self.visits}, UCB: {self.ucb}")
            for child_action, child_node in self.children.items():
                child_node.print_tree(depth + 1)



class AIPlayer:
    """
    AI 玩家
    """

    def __init__(self, color):
        """
        玩家初始化
        :param color: 下棋方，'X' - 黑棋，'O' - 白棋
        """

        self.color = color

    def get_move(self, board):
        """
        根据当前棋盘状态获取最佳落子位置
        :param board: 棋盘
        :return: action 最佳落子位置, e.g. 'A1'
        """
        if self.color == 'X':
            player_name = '黑棋'
        else:
            player_name = '白棋'
        print("请等一会，对方 {}-{} 正在思考中...".format(player_name, self.color))

        # -----------------请实现你的算法代码--------------------------------------
        action = None
        #获取当前棋盘的合法落子位置列表
        legal_actions = list(board.get_legal_actions(self.color))
        myboard = board
        #若没有合法落子位置，则返回None
        if len(legal_actions) == 0:
            return action
        elif len(legal_actions) == 1:
            return legal_actions[0]
        #进行蒙特卡洛树搜索
        root = TreeNode(color = self.color)
        root.ucb = -MY_CONSTANT
        for item in legal_actions:
            #item 是类似 E1 的 形式
            #test
            # print(item)
            root.children[item] = TreeNode(action = item, parent = root, color = self.color)

        #test
        # myboard.display()
        try:
            def circle_func():
                i = 0
                while True:
                    #test 
                    # root.print_tree()
                    #模拟过程中board会被修改
                    temp_board = deepcopy(myboard)

                    #选择
                    best_child = self.find_best_child(root)
                    while best_child.ucb != MY_CONSTANT and len(best_child.children) != 0:
                        temp_board._move(best_child.action, best_child.color)
                        self.calculate_ucb(best_child, i)
                        best_child = self.find_best_child(best_child)
                    #获得对手下棋表示
                    color_op = 'X' if best_child.color == 'O' else 'O'

                    if best_child.ucb != MY_CONSTANT and len(best_child.children) == 0:
                        #无棋可下或结局
                        #对手下棋
                        temp_board._move(best_child.action, best_child.color)
                        score = self.simulate(temp_board, color_op, self.color)
                    else:
                        # 更新
                        temp_board._move(best_child.action, best_child.color)
                        
                        #test
                        # print(best_child.action)

                        #扩展
                        legal_actions = list(temp_board.get_legal_actions(color_op))
                        if len(legal_actions) == 0:
                            #我继续下棋
                            score = self.simulate(temp_board, best_child.color, self.color)
                        else:
                            #建立树结构
                            for item in legal_actions:
                                best_child.children[item] = TreeNode(action = item, parent = best_child, color = color_op)
                            #模拟
                            #result 表示结果，minus表示相差的棋子数目
                            score = self.simulate(temp_board, color_op, self.color)

                    #test
                    # print(score)
                            
                    # win_color = 'X' if result == 0 else 'O'
                    #暂时以 1 / 0 作为返回分数
                    # score = 1 if best_child.color == win_color else 0

                    #反向传播
                    while best_child != root:
                        best_child.score = best_child.score + score
                        best_child.visits = best_child.visits + 1
                        #只起激活作用
                        best_child.ucb = 0 
                        best_child = best_child.parent 
                    # root.print_tree()
                    i += 1
                    self.calculate_ucb(root, i)
                    # root.print_tree()
                    # input()
                    # test
                    # input() 
            func_timeout(1, circle_func)
        #test
        # root.print_tree()
        # input()
        except FunctionTimedOut:    
            best_child = self.find_best_child(root) 
            action = best_child.action   
            # print(action)
        return action
    
    def calculate_ucb(self, root, total_visits):
        # self.ucb = negative * self.score / self.visits + ( 2 *  math.log(total_visits) / self.visits ) ** 0.5
        if len(root.children) == 0:
            return
        for child_action, child_node in root.children.items():
            if child_node.ucb != MY_CONSTANT:
                negative = 1 if child_node.color != self.color or child_node.parent.ucb == -MY_CONSTANT else -1
                child_node.ucb = negative * child_node.score / child_node.visits + ( 2 *  math.log(total_visits) / child_node.visits ) ** 0.5
                # self.calculate_ucb(child_node,total_visits) 

    #选择ucb最大的子节点
    def find_best_child(self, node):
        best_child = None
        for child in node.children:
            temp_child = node.children[child]
            if temp_child.ucb == MY_CONSTANT:
                best_child = temp_child
                break
            else:
                if best_child == None:
                    best_child = temp_child
                else:
                    if temp_child.ucb > best_child.ucb:
                        best_child = temp_child
        return best_child
    
    #模拟
    def random_simulate(self, board, color):
        flag = 0
        while flag < 2:
            legal_actions = list(board.get_legal_actions(color))
            if len(legal_actions) == 0:
                color = 'X' if color == 'O' else 'O'
                #一方无棋可走
                if flag == 0:
                    flag = 1
                elif flag == 1:
                    flag = 2
                continue
            flag = 0
            if color != self.color:
                action = random.choice(legal_actions)
            else:
            #策略1 random
                if self.color != "X":
                    action = random.choice(legal_actions)
            #策略2 选最大改变
                # max_num = 0
                # for item_action in legal_actions:
                #     filp_num = len(board._can_fliped(item_action, color))
                #     if filp_num > max_num:
                #         max_num = filp_num
                #         action = item_action
            #策略2.1
                # max_num = 0
                # max_num_out = 0
                # color_flip = 0
                # for item_action in legal_actions:
                #     flip_list = board._can_fliped(item_action, color)
                #     color_flip = len(flip_list)
                #     board._move(item_action, color)
                #     else_color = 'X' if color == 'O' else 'O'
                #     for item in list(board.get_legal_actions(else_color)):
                #         filp_num = len(board._can_fliped(item, else_color))
                #         if filp_num > max_num:
                #             max_num = filp_num
                #     color_flip = max(color_flip - max_num, 0)
                #     if color_flip >= max_num_out:
                #         max_num_out = color_flip
                #         action = item_action
                #     board.backpropagation(item_action, flip_list, color)
            #策略3 倾向于1/3/6/8行，1/3/6/8列
                else:
                    max_score = 0
                    for item_action in legal_actions:
                        if isinstance(item_action, str):
                            new_action = board.board_num(item_action)
                        else:
                            new_action = item_action
                        x_pos, y_pos = new_action
                        score = self.check_weight(x_pos)
                        score += self.check_weight(y_pos)
                        if score > max_score:
                            max_score = score
                            action = item_action
            #策略4 不要贴住自己的棋子
            # for item_action in legal_actions:
            #     flip_num = len(board._can_fliped(item_action, color))

            board._move(action, color)
            color = 'X' if color == 'O' else 'O'
            # input()
        #假定board不会发生变化
        #暂且仅用获胜与否作为返回值
        return board.get_winner()
    
    #判断权重
    def check_weight(self, x_pos):
        total_score = 0
        if(x_pos == 0 or x_pos == 7):
            total_score += 10
        elif(x_pos == 1 or x_pos == 6):
            total_score += 3
        elif(x_pos ==  2 or x_pos == 5):
            total_score += 5
        elif(x_pos == 3 or x_pos == 4):
            total_score += 1
        return total_score
    
    #模拟
    def simulate(self, board, color, color_need_win):
        success = 0
        total_minus = 0
        temp_board = deepcopy(board)
        result , minus = self.random_simulate(temp_board, color)
        if result == 0:
            win_color = 'X'
        elif result == 1:
            win_color = 'O'
        else:
            win_color = None

        if color_need_win == win_color:
            score = 2
        elif win_color == None:
            score = 1
        else:
            score = 0
        return score
            