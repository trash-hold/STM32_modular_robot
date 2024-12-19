import math as m

if __name__ == '__main__':
    x = -5
    y = 23
    z = 121

    theta = m.degrees( m.atan(x / m.sqrt(y**2 + z**2)))
    psi = m.degrees( m.atan(y / m.sqrt(x**2 + z**2)))
    phi = m.degrees( m.atan(m.sqrt(x**2 + y**2) / z))

    print(m.sqrt(y**2 + z**2))
    print(m.sqrt(x**2 + z**2))
    print(m.sqrt(x**2 + y**2))
    print("theta: {0}, psi {1}, phi {2}".format(theta, psi, phi))