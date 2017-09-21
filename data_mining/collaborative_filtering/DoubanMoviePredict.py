from math import sqrt
import sys
import codecs


class DoubanMoviePredictor:
    def __init__(self, metric='pearson'):
        self.metric = metric
        if metric == 'pearson':
            self.fn = self.pearson
        elif metric == 'manhattan':
            self.fn = self.manhattan
        elif metric == 'euclidean':
            self.fn = self.euclidean
        elif metric == 'cosin':
            self.fn = self.cosin
        self.train = {}
        self.test = []
        self.near_user_map = {}

    @staticmethod
    def pearson(x_ratings, y_ratings):
        sum_xy = 0
        sum_x = 0
        sum_y = 0
        sum_x2 = 0
        sum_y2 = 0
        n = 0
        for movie in x_ratings:
            if movie in y_ratings:
                x = x_ratings[movie]
                y = y_ratings[movie]
                n += 1
                sum_xy += x * y
                sum_x += x
                sum_y += y
                sum_x2 += x ** 2
                sum_y2 += y ** 2
        if n == 0:
            return 0
        denomitor = sqrt(sum_x2 - (sum_x ** 2) / n) * sqrt(sum_y2 - (sum_y ** 2) / n)
        if denomitor == 0:
            return 0
        return (sum_xy - (sum_x * sum_y) / n) / denomitor

    @staticmethod
    def manhattan(x_ratings, y_ratings):
        distance = 0
        n = 0
        for movie in x_ratings:
            if movie in y_ratings:
                n += 1
                distance += abs(x_ratings[movie] - y_ratings[movie])
        if n == 0:
            return sys.maxsize
        return distance / n

    @staticmethod
    def euclidean(x_ratings, y_ratings):
        distance = 0
        n = 0
        for movie in x_ratings:
            if movie in y_ratings:
                n += 1
                distance += (x_ratings[movie] - y_ratings[movie]) ** 2
        if n == 0:
            return sys.maxsize
        return sqrt(distance)

    @staticmethod
    def cosin(x_ratings, y_ratings):
        sum_xy = 0
        sum_x2 = 0
        sum_y2 = 0
        n = 0
        for movie in x_ratings:
            if movie in y_ratings:
                n += 1
                x = x_ratings[movie]
                y = y_ratings[movie]
                sum_xy += x * y
                sum_x2 += x ** 2
                sum_y2 += y ** 2
        if n == 0:
            return -1
        else:
            return sum_xy / (sqrt(sum_x2) * sqrt(sum_y2))

    def compute_nearest_user(self, user):
        near_users = []
        for eachUser in self.train:
            if eachUser != user:
                distance = self.fn(self.train[user],
                                   self.train[eachUser])
                near_users.append((eachUser, distance))
        if self.metric == 'pearson' or self.metric == 'cosin':
            near_users.sort(key=lambda the_tuple: the_tuple[1], reverse=True)
        else:
            near_users.sort(key=lambda the_tuple: the_tuple[1], reverse=False)
        return near_users

    def load_data(self, path=''):
        f = codecs.open(path + "train.txt", 'r', 'utf8')
        i = 0
        self.train = {}
        for line in f:
            i += 1
            if i == 1:
                continue
            # separate line into fields
            fields = line.split(',')
            user = fields[0]
            movie = fields[1]
            rating = int(fields[2].strip())
            if user in self.train:
                current_ratings = self.train[user]
            else:
                current_ratings = {}
            current_ratings[movie] = rating
            self.train[user] = current_ratings
        f.close()
        print("Read train data lines: ", i)
        f = codecs.open(path + "test.txt", 'r', 'utf8')
        i = 0
        self.test = []
        for line in f:
            i += 1
            if i == 1:
                continue
            # separate line into fields
            fields = line.split(',')
            user = fields[0]
            movie = fields[1].strip()
            self.test.append((user, movie))
        f.close()
        print("Read data lines: ", i)

    def predict(self):
        result = ''
        for (user, movie) in self.test:
            rating = 3
            if user not in self.near_user_map:
                self.near_user_map[user] = self.compute_nearest_user(user)
            near_users = self.near_user_map[user]
            for (near_user, distance) in near_users:
                if movie not in self.train[near_user]:
                    continue
                rating = self.train[near_user][movie]
                break
            result += str(rating)
        return result


r = DoubanMoviePredictor('cosin')
r.load_data('/home/ryan/Downloads/')
print(r.predict())
