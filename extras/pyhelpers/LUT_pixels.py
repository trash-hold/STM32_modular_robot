from PIL import Image

if __name__ == "__main__":
    px_block_size = 14

    # Define color palette
    red = (182,0,0)
    white = (255, 255, 255)
    light_gray = (161, 161, 161)
    dark_gray = (84, 84, 84)
    black = (0, 0, 0)

    # Open picture
    img = Image.open("res/gizmo_black_white.jpg")
    pix = img.load()

    width, height = img.size

    # Get coordinates for each color
    white_px = []
    light_gray_px = []
    dark_gray_px = []
    max_x = 0
    max_y = 0 

    for i in range(0, height, px_block_size):
        for j in range(0, width, 14):
            a = i//14
            b = j//14
            if pix[j, i] == white:
                white_px.append([b, a])
            elif pix[j, i] == light_gray:
                light_gray_px.append([b, a])
            elif pix[j, i] == dark_gray:
                dark_gray_px.append([b, a])
            else:
                continue

            if b > max_x:
                max_x = b
            if a > max_y:
                max_y = a
        
            

    print("Number of pixels: {0}".format(str(len(white_px) +  len(light_gray_px) + len(dark_gray_px))))
    print("Max x: {0}, Max y: {1}".format(max_x, max_y))

    offset_x = (128 - max_x)//2
    offset_y = (160 - max_y)//2

    print("Offset x: {0}, Offset y: {1}".format(offset_x, offset_y))

    white_LUT = '''uint16_t white_px_LUT[] = { '''
    l_grey_LUT = '''uint16_t l_grey_px_LUT[] = { '''
    d_grey_LUT = '''uint16_t d_grey_px_LUT[] = { '''

    colors_px = [white_px, light_gray_px, dark_gray_px]
    colors_lut = [white_LUT, l_grey_LUT, d_grey_LUT]

    for px_table, lut in zip(colors_px, colors_lut):
        for i in px_table:
            lut += str(i[0] + offset_x) + ", " + str(i[1] + offset_y) + ', '
        lut += " };"

        print(lut)


