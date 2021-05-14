# Homework 2-9
import property as P
tc = P.house('apartment', 5600000, 'tclin','taichung')
money = P.deposit('money', 100000, 'taiwanBank')
stock = P.stock('tpower', 5, 20000)
total = [tc, money, stock]
sum = 0
for i in total:
    sum = sum + i.getValue();
print("Total value of properties : {0}".format(sum))
