import sys
import os
import re
from pprint import pprint

def usage():
    print("\nUsage:")
    print("    convert_atlas.py [libGDX_packer_output_dir]\n")
    exit(1)

if len(sys.argv) != 2:
    usage()

if not os.path.isdir(sys.argv[1]):
    usage()

atlas_file = None
for fn in os.listdir(sys.argv[1]):
    if fn.endswith(".atlas") and not fn.endswith(".allegro.atlas"): 
        atlas_file = fn
        break

if atlas_file is None:
    print("\nError: Unable to find atlas file in {}\n".format(sys.argv[1]))
    exit(1)

f = open("{}/{}".format(sys.argv[1], atlas_file))
atlas_text = f.read()
f.close()

# Use a finite state machine to process the libGDX atlas so we can
# do it in 1 pass
fsm = {
    "in_image_section": False,
    "previous_line": None,
    "current_file_name": None,
    "current_image_name": None,
    "current_image_x": None,
    "current_image_y": None,
    "current_image_w": None,
    "current_image_h": None,
    "current_image_idx": None,
    "image_info": {}
}

# Handle all image metadata we care about
def handle_image_section_line(line):

    # get x and y coords
    if re.match('^\s\sxy.*', line):
        s = line.replace('  xy: ', '')
        xy = s.split(',')
        fsm['image_info'][fsm['current_file_name']][fsm['current_image_name']]["x"] = int(xy[0].strip())
        fsm['image_info'][fsm['current_file_name']][fsm['current_image_name']]["y"] = int(xy[1].strip())

    # get size
    elif re.match('^\s\ssize.*', line):
        s = line.replace('  size: ', '')
        wh = s.split(',')
        fsm['image_info'][fsm['current_file_name']][fsm['current_image_name']]["w"] = int(wh[0].strip())
        fsm['image_info'][fsm['current_file_name']][fsm['current_image_name']]["h"] = int(wh[1].strip())

    # get index
    elif re.match('^\s\sindex.*', line):
        s = line.replace('  index: ', '')
        fsm['image_info'][fsm['current_file_name']][fsm['current_image_name']]["idx"] = int(s.strip())
    
    return


for line in atlas_text.splitlines():

    # starting a new file
    if re.match('.+\.(png|jpg)$', line):
        fsm['in_image_section'] = False
        fsm['current_file_name'] = line
        fsm['current_image_name'] = None
        fsm['current_image_x'] = None
        fsm['current_image_y'] = None
        fsm['current_image_w'] = None
        fsm['current_image_h'] = None
        fsm['current_image_idx'] = None
        fsm['image_info'][fsm['current_file_name']] = {}

    # entered new image section, previous line is the image name
    elif not fsm['in_image_section'] and re.match('^\s\s.+$', line):
        fsm['in_image_section'] = True
        fsm['current_image_name'] = fsm['previous_line']
        fsm['image_info'][fsm['current_file_name']][fsm['current_image_name']] = {}
        handle_image_section_line(line)

    # In the image section, handle the line
    elif fsm['in_image_section'] and re.match('^\s\s.+$', line):
        handle_image_section_line(line)

    # no longer in an image section as line doesn't start with 2 spaces
    # correct previous image name by appending the index if one exists
    elif fsm['in_image_section'] and re.match('^[^\s].+$', line):
        fsm['in_image_section'] = False
        content = fsm['image_info'][fsm['current_file_name']][fsm['current_image_name']]
        if content['idx'] >= 0:
            new_key = "{}_{}".format(fsm['current_image_name'], content['idx'])
            fsm['image_info'][fsm['current_file_name']][new_key] = content
            fsm['image_info'][fsm['current_file_name']].pop(fsm['current_image_name'])

    fsm['previous_line'] = line

# Now, we write out new file
allegro_atlas_abs_file_name = "{}/{}.allegro.atlas".format(sys.argv[1], atlas_file.replace('.atlas', ''))

all_file_names = ','.join(fsm['image_info'].keys())

if os.path.exists(allegro_atlas_abs_file_name):
    os.remove(allegro_atlas_abs_file_name)

f = open(allegro_atlas_abs_file_name, 'w')

# Iterate through all the images
file_count = 0
image_count = 0
for fn in fsm['image_info']:
    file_count += 1
    for image_name in fsm['image_info'][fn]:
        f.write('[{}]\n'.format(image_name))
        f.write('file={}\n'.format(fn))
        content = fsm['image_info'][fn][image_name]
        f.write('x={}\n'.format(content['x']))
        f.write('y={}\n'.format(content['y']))
        f.write('w={}\n'.format(content['w']))
        f.write('h={}\n'.format(content['h']))
        f.write('id={}\n'.format(image_count))
        image_count += 1

f.write('\n[file_metadata]\n'.format(all_file_names))
f.write('files={}\n'.format(all_file_names))
f.write('total_files={}\n'.format(file_count))
f.write('total_images={}\n'.format(image_count))
f.close()

print("\nSuccessfully processed atlas!\n\nCreated {}\n".format(allegro_atlas_abs_file_name))