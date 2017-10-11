#!/usr/bin/python3

import numpy as np
from scipy.sparse.linalg import svds
from scipy import sparse
from math import sqrt


def vector_to_diagonal(vector):
    """
    将向量放在对角矩阵的对角线上
    :param vector:
    :return:
    """
    if (isinstance(vector, np.ndarray) and vector.ndim == 1) or \
            isinstance(vector, list):
        length = len(vector)
        diag_matrix = np.zeros((length, length))
        np.fill_diagonal(diag_matrix, vector)
        return diag_matrix
    return None


data = {}
users = []
movies = []
user_avgs = []
movie_avgs = []

# Load data
train_file = 'train.txt'
with open(train_file, 'r') as f:
    next(f)
    for line in f:
        fields = line.strip().split(',')
        user = fields[0]
        movie = fields[1]
        rating = int(fields[2])
        if user not in users:
            users.append(user)
        if movie not in movies:
            movies.append(movie)
        data.setdefault(user, {})
        data[user][movie] = rating

# Generate original rating matrix
user_count = len(users)
movie_count = len(movies)
R = np.zeros((user_count, movie_count))
for (user, ratings) in data.items():
    for (movie, rating) in ratings.items():
        user_idx = users.index(user)
        movie_idx = movies.index(movie)
        R[user_idx][movie_idx] = rating

# Calculate user average rating
for user_idx in range(user_count):
    user_ratings = R[user_idx, :]
    n = 0
    sum_rating = 0.0
    for rating in user_ratings:
        if rating != 0:
            n += 1
            sum_rating += rating
    user_avgs.append(sum_rating / n)

# Calculate movie average rating
for movie_idx in range(movie_count):
    movie_ratings = R[:, movie_idx]
    n = 0
    sum_rating = 0.0
    for rating in movie_ratings:
        if rating != 0:
            n += 1
            sum_rating += rating
    movie_avgs.append(sum_rating / n)

# Normalize rating matrix R
R_normal = R
for i in range(user_count):
    for j in range(movie_count):
        if R_normal[i][j] == 0:
            R_normal[i][j] = user_avgs[i]
for i in range(user_count):
    for j in range(movie_count):
        R_normal[i][j] -= movie_avgs[j]

# SVD R_normal
R_normal = R_normal.astype('float')
U, S, VT = svds(sparse.csr_matrix(R_normal), k=2, maxiter=200)  # 2个隐主题
S = vector_to_diagonal(S)
S_Sqrt = S
for i in range(S.ndim):
    S_Sqrt[i][i] = sqrt(S[i][i])
R_red = np.dot(np.dot(U, S), VT)

# Calculate movie similarity matrix
S_Sqrt_VT = np.dot(S_Sqrt, VT)
sim = {}
for i in range(movie_count - 1):
    sim.setdefault(i, [])
    for j in range(i + 1, movie_count):
        sim.setdefault(j, [])
        sum_xy = 0
        sum_x2 = 0
        sum_y2 = 0
        for k in range(S.ndim):
            sum_xy += S_Sqrt_VT[k][i] * S_Sqrt_VT[k][j]
            sum_x2 += S_Sqrt_VT[k][i] ** 2
            sum_y2 += S_Sqrt_VT[k][j] ** 2
        simij = sum_xy / sqrt(sum_x2 * sum_y2)
        sim[i].append((simij, j))
        sim[j].append((simij, i))

for (key, val) in sim.items():
    val.sort(key=lambda tup: tup[0], reversed=True)


def predict_rating(user_idx, movie_idx, l):
    numerator = 0
    denominator = 0
    a = user_idx
    j = movie_idx
    for k in range(l):
        simjk = sim[j][k][0]
        sim_movie_idx = sim[j][k][1]
        numerator += simjk * (R_red[a][sim_movie_idx] + user_avgs[a])
        denominator += abs(simjk)
    if denominator == 0:
        return 3
    else:
        return numerator / denominator


test_file = 'test.txt'
result = ''
with open(test_file, 'r') as f:
    next(f)
    for line in f:
        fields = line.strip().split(',')
        user = fields[0]
        movie = fields[1]
        if user in users and movie in movies:
            user_idx = users.index(user)
            movie_idx = movies.index(movie)
            rating = predict_rating(user_idx, movie_idx, 3)
            if rating < 1:
                rating = 1
            elif rating > 5:
                rating = 5
        elif user in users:
            rating = int(round(user_avgs[users.index(user)]))
        else:
            rating = 3
        result += str(rating)

print(result)
