import imageio, argparse
parser = argparse.ArgumentParser()
parser.add_argument("--size")
parser.add_argument("--dir")
parser.add_argument("--freq")
args = parser.parse_args()
images = []
for i in range(int(args.freq), int(args.size) + 1, int(args.freq)):
    images.append(imageio.imread(args.dir + "/" + str(i) + ".bmp"))
imageio.mimsave(args.dir + "/" + 'res.gif', images)
