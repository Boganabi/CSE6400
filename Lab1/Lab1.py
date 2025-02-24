from queue import Queue, PriorityQueue

class Solution:

    def __init__(self):
        self.lines = self.parse_file()

        self.alg = self.lines[0]
        self.start = self.lines[1]
        self.end = self.lines[2]

        self._line_num = int(self.lines[3])
        self.frontier, self.weights = self.parse_frontier(4, self._line_num + 4)

        self._line_heuristic_num = int(self.lines[self._line_num + 4])
        self.heuristic = self.parse_heuristic(self._line_num + 5, self._line_num + self._line_heuristic_num + 5)

    def parse_file(self) -> list:
        with open("input.txt") as file:
            lines = [line.rstrip() for line in file]
        return lines

    def parse_frontier(self, parse_start: int, parse_end: int) -> tuple[dict, dict]:
        d = dict()
        w = dict()
        for i in range(parse_start, parse_end):
            items = self.lines[i].split()
            if(items[0] in d.keys()):
                d[items[0]].append(items[1])
            else:
                d[items[0]] = [items[1]]
            w[(items[0], items[1])] = int(items[2])
        return d, w

    def parse_heuristic(self, parse_start: int, parse_end: int) -> dict:
        d = dict()
        for i in range(parse_start, parse_end):
            items = self.lines[i].split()
            # since there are always 2 items, we can make dict like this
            d[items[0]] = int(items[1])
        return d

    def write_solution(self, path: list) -> None:
        f = open("output.txt", "w")
        cost = 0
        last_node = ""
        for i in path:
            if(i != self.start):
                cost += self.weights[(last_node, i)]
            f.write(i + " " + str(cost) + "\n")
            last_node = i
        f.close()
    
    def get_path_cost(self, parent: str, child: str) -> int:
        # need to get cost both ways
        if (parent, child) not in self.weights.keys():
            return self.weights[(child, parent)]
        else:
            return self.weights[(parent, child)]

    def start_search(self) -> None:
        # print(self.frontier)
        # print(self.weights)
        # print(self.heuristic)
        if(self.alg == "BFS"):
            self.bfs()
        if(self.alg == "DFS"):
            self.dfs()
        if(self.alg == "UCS"):
            self.ucs()
        if(self.alg == "A*"):
            self.astar()

    def bfs(self) -> None:
        if self.start == self.end:
            self.write_solution([self.start])
            return

        q = Queue()
        # insert starting node and path
        q.put((self.start, [self.start]))
        explored = set()

        while not q.empty():
            node, path = q.get()
            explored.add(node)
            for n in self.frontier[node]:
                if n not in explored:
                    if n == self.end:
                        self.write_solution(path + [n])
                        return
                    q.put((n, path + [n]))

    def dfs(self) -> None:
        if self.start == self.end:
            self.write_solution([self.start])
            return
        
        stack = []
        stack.append((self.start, [self.start]))
        explored = set()

        while len(stack) != 0:
            node, path = stack.pop()
            explored.add(node)
            for n in self.frontier[node]:
                if n not in explored:
                    if n == self.end:
                        self.write_solution(path + [n])
                        return
                    stack.append((n, path + [n]))

    def ucs(self) -> None:
        if self.start == self.end:
            self.write_solution([self.start])
            return
        
        q = PriorityQueue()
        explored = set()
        q.put((0, self.start, [self.start]))
        while q.not_full:
            cost, node, path = q.get()
            if node == self.end:
                self.write_solution(path)
                return
            explored.add(node)
            for n in self.frontier[node]:
                new_cost = cost + self.get_path_cost(node, n)
                
                if n not in explored:
                    q.put((new_cost, n, path + [n]))
                # replace with lower cost, if applicable
                for i, (c, child, p) in enumerate(q.queue):
                    if child == n and c >= new_cost:
                        q.queue[i] = (new_cost, n, path + [n])

    def astar(self) -> None:
        if self.start == self.end:
            self.write_solution([self.start])
            return
        
        q = PriorityQueue()
        explored = set()
        q.put((self.heuristic[self.start], self.start, [self.start]))
        while q.not_full:
            cost, node, path = q.get()
            if node == self.end:
                self.write_solution(path)
                return
            explored.add(node)
            for n in self.frontier[node]:
                new_cost = cost + self.get_path_cost(node, n) + self.heuristic[n]
                
                if n not in explored:
                    q.put((new_cost, n, path + [n]))
                # replace with lower cost, if applicable
                for i, (c, child, p) in enumerate(q.queue):
                    if child == n and c >= new_cost:
                        q.queue[i] = (new_cost, n, path + [n])


if __name__ == "__main__":
    s1 = Solution()
    s1.start_search()
    # output will now be in output.txt