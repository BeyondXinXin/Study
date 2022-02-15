# 学习numpy
# https://www.bilibili.com/video/BV19T4y127Z2?p=1

import numpy as np


# 第一节 创建数组
def create_array():
    print("---- Create Array ----")

    arr1 = np.array([1, 2, 3, 4])
    print(arr1)

    arr2 = [5, 6, 7, 8]
    arr2np = np.array(arr2)
    print(type(arr2))
    print(type(arr2np))

    arr_zero = np.zeros((3, 4))
    print(arr_zero)

    arr_zero = np.zeros((3, 4), dtype=np.int32)
    print(arr_zero)

    arr_one = np.ones((4, 5))
    print(arr_one)

    arr_empty = np.empty((5, 6))
    print(arr_empty)

    arr_eye = np.eye(4)
    print(arr_eye)

    arr_even = np.arange(1.2, 20, 2)
    print(arr_even)

    arr2d = [[1, 2, 3], [4, 5, 6]]
    arr2dnp = np.array(arr2d)
    print(arr2dnp)
    print(arr2dnp.shape)

    arr = np.arange(12).reshape(3, 4)
    print(arr)
    arr = np.ones_like(arr)
    print(arr)


# 第二节 数组的基本操作
def array_basic_operation():
    print("---- Array Basic Operation ----")

    a = np.array([5, 5, 5])
    b = np.array([10, 10, 10])
    c = np.array([5, 10, 15])
    print(a + b, a - b, a * b, a / b)
    print(a % b)
    print(c > 0)
    print(c > 10)
    print(c > a)

    aa = np.array([[1, 1], [0, 1]])
    bb = np.array([[2, 0], [3, 4]])
    print(aa * bb, aa.dot(bb), np.dot(aa, bb))
    print(aa.dot(bb), np.dot(aa, bb))

    d = np.array([23, 14, 89, 35, 20])
    print(d.sum(), d.max(initial=100), d.min(initial=0))

    n = np.array([[0, 1, 2, 3],
                  [4, 5, 6, 7],
                  [8, 9, 10, 11]])
    print(n.sum(), n.sum(axis=0), n.sum(axis=1))
    print(n.max(initial=100), n.max(axis=0, initial=100), n.max(axis=1, initial=100))
    print(n.min(initial=100), n.min(axis=0, initial=100), n.min(axis=1, initial=100))


# 第三节 通用函数
def general_function():
    print("---- General Function ----")
    angles = np.array([0, 30, 45, 60, 90])
    radians = angles * np.pi / 180
    print(radians, np.radians(angles))
    print(np.sin(np.radians(angles)), np.cos(np.radians(angles)))
    print(np.degrees(np.arcsin(np.sin(np.radians(angles)))))

    nums = np.array([12.34, 23.76, 34.35, 36.11, 65.23, 98.23, 36.44, 57.56, 81.44, 45.01])
    print(np.mean(nums), np.median(nums), np.std(nums))

    salaries = np.genfromtxt("./salary.csv", delimiter=",")
    print(salaries)


# 第四节 索引和切片
def indexes_and_section():
    print("---- Indexes And Section ----")
    a = np.array([0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100])
    print(a[2], a[-2], a[2:5])
    print(a[::-1])

    s = np.array([["Dave", "Nick", "Martin", "Sarah"],
                  [98, 75, 65, 99],
                  [96, 77, 89, 100]])
    print(s[0], s[0, 1], s[0:2, 2:3])
    print(s[:, 2:4])


# 第五节 遍历
def ergodic():
    print("---- Ergodic -----")
    a = np.arange(11) ** 2
    print(a)

    for i in a:
        print(i ** .5)

    students = np.array([["Dave", "Nick", "Martin", "Sarah"],
                         [98, 75, 65, 99],
                         [96, 77, 89, 100]])
    for s in students:
        print("s=", s)
    for s in students.flatten(order="F"):
        print(s)

    x = np.arange(12).reshape(3, 4)
    print(x)
    for i in np.nditer(x):
        print(i)

    for i in np.nditer(x, order="F"):
        print(i)

    for i in np.nditer(x, order="F", flags=["external_loop"]):
        print(i)

    for i in np.nditer(x, op_flags=["readwrite"]):
        i[...] = i ** 2
    print(x)


# 第六节 塑形
def reshape():
    print("---- Reshape ----")

    a = np.array([("a", "b", "c", "d"), ("e", "f", "g", "h")])
    print(a, a.shape, a.ravel())
    print(a.T, a.T.ravel())
    print(a.reshape(4, 2))

    print(np.arange(15).reshape(3, 5))
    print(np.arange(15).reshape(5, 3))
    print(np.arange(15).reshape(3, -1))
    print(np.arange(15).reshape(-1, 5))


# 第七节 分割数组
def split_array():
    print("---- Split Array ----")
    x = np.arange(9)
    print(x)
    print(np.split(x, 3))
    print(np.split(x, [3, 4]))

    x = np.arange(16).reshape(4, 4)
    print(x)
    print(np.hsplit(x, 2))
    print(np.vsplit(x, 2))


if __name__ == '__main__':
    split_array()
