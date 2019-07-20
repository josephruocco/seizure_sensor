# 2019, Raymond Blum <raygeeknyc@gmail.com>
# Read smoothed values from a photosensor


'''
For reference:
A way to gather accurate data but getting an average that ignores min and max vals
'''

def smooth_light_level(sample_count):
    total = 0
    min = 99999
    max = 0
    for i in range(sample_count):
        light_sample = photocell.value
        if light_sample > max:
            max = light_sample
        if light_sample < min:
            min = light_sample
        total += light_sample
    total -= min
    total -= max
    return int(total / (sample_count-2))
