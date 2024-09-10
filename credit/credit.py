import re
import cs50


def isAmex(number):
    if re.match(r'^3(4|7)\d{13}$', number):
        return True
    else:
        return False


def isVisa(number):
    if re.match(r'^4\d{12}(\d{3})?$', number):
        return True
    else:
        return False


def isMaster(number):
    if re.match(r'^5[1-5]\d{14}$', number):
        return True
    else:
        return False


def luhn(number):
    sum = 0
    number = list(number)

    while len(number) > 0:
        sum += int(number.pop())
        if len(number) == 0:
            break
        temp = int(number.pop()) * 2
        sum += temp // 10 + temp % 10

    if sum % 10 == 0:
        return True
    else:
        return False


while True:
    number = cs50.get_string("Number: ")
    if number.isdigit():
        break

if luhn(number):
    if isAmex(number):
        print("AMEX")
    elif isVisa(number):
        print("VISA")
    elif isMaster(number):
        print("MASTERCARD")
    else:
        print("INVALID")
else:
    print("INVALID")
