import colorScale

"""
QMap<float, rgb> name;
name[index] = rgb(0, 0,0);
"""

for key, val in colorScale.listOfmaps.items():
    topList = []
    name = "{0}Map".format("_".join(key.split('-')))
    mapstr =  "std::map<FloatKey, rgb> {0};\n".format(name)
    topList.append(mapstr)
    
    for k in val:
        rgbMapdict = {}
        rgbMapdict["name"] = name
        rgbMapdict["index"] = k["index"]
        rgbMapdict["r"] = k["rgb"][0]
        rgbMapdict["g"] = k["rgb"][1]
        rgbMapdict["b"] = k["rgb"][2]

        rgbMapdictstr = "{name}[{{{index}}}] = rgb({r}, {g}, {b});\n".format_map(rgbMapdict)
        topList.append(rgbMapdictstr)
    registerStr = 'cTable->registerMap("{0}", {1});\n'.format(key.capitalize(), name)
    topList.append(registerStr)
        
    print ("".join(topList))
