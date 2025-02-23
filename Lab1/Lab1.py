

class Solution:

    def __init__(self):
        self.lines = self.parse_file()

        self.alg = self.lines[0]
        self.start = self.lines[1]
        self.end = self.lines[2]

        self._line_num = int(self.lines[3])
        self.frontier = self.parse_frontier(4, self._line_num + 4)

        self._line_heuristic_num = int(self.lines[self._line_num + 4])
        self.heuristic = self.parse_heuristic(self._line_num + 5, self._line_num + self._line_heuristic_num + 5)

    def parse_file(self) -> list:
        with open("input.txt") as file:
            lines = [line.rstrip() for line in file]
        return lines

    def parse_frontier(self, parse_start: int, parse_end: int) -> dict:
        d = dict()
        for i in range(parse_start, parse_end):
            items = self.lines[i].split()
            # since there are always 3 items, we can make dict like this
            d[items[0] + items[1]] = items[2]
        return d

    def parse_heuristic(self, parse_start: int, parse_end: int) -> dict:
        d = dict()
        for i in range(parse_start, parse_end):
            items = self.lines[i].split()
            # since there are always 3 items, we can make dict like this
            d[items[0]] = items[1]
        return d

    def start_search(self) -> None:
        if(self.alg == "BFS"):
            self.bfs()
        if(self.alg == "DFS"):
            self.dfs()
        if(self.alg == "UCS"):
            self.ucs()
        if(self.alg == "A*"):
            self.astar()

    def bfs(self) -> None:
        pass

    def dfs(self) -> None:
        pass

    def ucs(self) -> None:
        pass

    def astar(self) -> None:
        pass


if __name__ == "__main__":
    s1 = Solution()
    s1.start_search()
    # output will now be in output.txt