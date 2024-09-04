import copy
# 机器人移动方向
move_map = {
    'u': (-1, 0), # up
    'r': (0, +1), # right
    'd': (+1, 0), # down
    'l': (0, -1), # left
}
# 迷宫路径搜索树
class SearchTree(object):
    def __init__(self, loc=(), action='', parent=None):
        """
        初始化搜索树节点对象
        :param loc: 新节点的机器人所处位置
        :param action: 新节点的对应的移动方向
        :param parent: 新节点的父辈节点
        """
        self.loc = loc  # 当前节点位置
        self.to_this_action = action  # 到达当前节点的动作
        self.parent = parent  # 当前节点的父节点
        self.children = []  # 当前节点的子节点
    def add_child(self, child):
        """
        添加子节点
        :param child:待添加的子节点
        """
        self.children.append(child)
    def is_leaf(self):
        """
        判断当前节点是否是叶子节点
        """
        return len(self.children) == 0
def expand(maze, is_visit_m, node):
    """
    拓展叶子节点，即为当前的叶子节点添加执行合法动作后到达的子节点
    :param maze: 迷宫对象
    :param is_visit_m: 记录迷宫每个位置是否访问的矩阵
    :param node: 待拓展的叶子节点
    """
    can_move = maze.can_move_actions(node.loc)
    for a in can_move:
        new_loc = tuple(node.loc[i] + move_map[a][i] for i in range(2))
        if not is_visit_m[new_loc]:
            child = SearchTree(loc=new_loc, action=a, parent=node)
            node.add_child(child)
#             没有刷新is_visit_m矩阵
def back_propagation(node):
    """
    回溯并记录节点路径
    :param node: 待回溯节点
    :return: 回溯路径
    """
    path = []
    while node.parent is not None:
        path.insert(0, node.to_this_action)
        node = node.parent
    return path
def breadth_first_search(maze, root, path, is_visit_m):
    """
    对迷宫进行广度优先搜索
    :param maze: 待搜索的maze对象
    """
    current_node = root
    is_visit_m[current_node.loc] = 1  # 标记当前节点位置已访问
    if current_node.loc == maze.destination:  # 到达目标点
        path = back_propagation(current_node)
        return 1
    if current_node.is_leaf():
        expand(maze, is_visit_m, current_node)
        for child in current_node.children:
            # 深度拷贝maze和is_visit_m
            copy_maze = copy.deepcopy(maze)
            copy_is_visit_m = copy.deepcopy(is_visit_m)
            if breadth_first_search(copy_maze, child, path, copy_is_visit_m) == 1:
                return 1
    return 0

def my_search(maze):
    """
    任选深度优先搜索算法、最佳优先搜索（A*)算法实现其中一种
    :param maze: 迷宫对象
    :return :到达目标点的路径 如：["u","u","r",...]
    """
    start = maze.sense_robot()
    root = SearchTree(loc=start)
    h, w, _ = maze.maze_data.shape
    is_visit_m = np.zeros((h, w), dtype=np.int)  # 标记迷宫的各个位置是否被访问过
    path = []  # 记录路径
    breadth_first_search(maze, root, path, is_visit_m)
    return path