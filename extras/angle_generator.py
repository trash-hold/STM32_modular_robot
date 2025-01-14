import random

if __name__ == '__main__':
    float_values = [random.random() for i in range(0,100)]
    sin_values = '{ '
    angle_values = '{ '

    for i in float_values:
        sin_values += f'{i: 0.4f}, '
        angle_values += f'{i*100: 0.4f}, '

    sin_values += ' };'
    angle_values += ' };'

    print(sin_values)

    print(angle_values)
    