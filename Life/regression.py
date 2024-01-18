from numpy import sqrt 
a = [.625,  .633]
b = [.391,  .405]
c = [.248,  .261] 
d = [.171, .182]

x = [10, 25, 40, 50]
y = [a, b, c, d]
my = [sum(a)/2, sum(b)/2, sum(c)/2, sum(d)/2]

def fxn(A,B, X):
    C = A/B
    return(C*(1/X))
    
def ls(i, j):
    h = (i-j)**2
    return(h)

f = 1
A = 2.0
B = 3.0
list = [0, 1,2,3]
re = [0.0,0.0,0.0,0.0]
ans = [.0, .0, .0, .0]
control = [0]
index = 0
vec = []
n= .01
f= 10
while f > .5:
    
    for i in list:
        re[i] = fxn(A,B, x[i])
        ans[i] = ls(re[i], my[i])
        ans[i] = sqrt(ans[i])
    vec.append(sqrt(sum(ans))) 
    
    if index != 0:
        print(vec[index])
        if vec[index] > vec[index - 1]:
            if control[index] == 0:
                B = B + n
                control.append(1)
            else:
                B = B - n
                control.append(0)
        if vec[index] < vec[index - 1]:
            if control[index] == 0:
                B = B + n
                control.append(1)
            else:
                B = B - n
                control.append(0)
    else:
        B = B + .001
        control.append(1)
    f = vec[index]
    index = index + 1

print(B)
print(A)