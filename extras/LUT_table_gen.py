import math as m

RANGE = 91

def interpolate(lut_table: dict, sin_val: float) -> float: 
    
    for i in lut_table.keys():
        if lut_table[i] == sin_val:
            return i 
        elif lut_table[i] > sin_val:
            big_angle = lut_table[i]
            small_angle = lut_table[i - 1]

            a = (big_angle - small_angle)
            b = small_angle - a * (i-1)

            interpolation = (sin_val - b) / a

            return interpolation

if __name__ == '__main__':
    LUT_table:str = """{ """
    value_table = dict()

    for i in range(0, RANGE):
        val = m.sin( m.radians(i))
        value_table[i] = val

        if i == 90:
            LUT_table += "{0: .5f}".format(val)
        else:
            LUT_table += "{0: .5f}, ".format(val)
    LUT_table += " };"

    # Checking interpolation equations
    angle1 = 22.5
    check1 = m.sin(m.radians(angle1))
    print(check1)
    angle1_inter = interpolate(value_table, check1)

    print("Input: {0}, Output: {1}".format(angle1, angle1_inter))
    print(LUT_table)

    LUT_table:str = """{ """
    value_table = dict()

    for i in range(0, RANGE):
        val = m.tan( m.radians(i))
        value_table[i] = val

        if i == 90:
            LUT_table += "{0: .4f}".format(val)
        else:
            LUT_table += "{0: .4f}, ".format(val)
    LUT_table += " };"

    # Checking interpolation equations
    angle1 = 22.5
    check1 = m.tan(m.radians(angle1))
    angle1_inter = interpolate(value_table, check1)

    print("Input: {0}, Output: {1}".format(angle1, angle1_inter))

    print(LUT_table)
