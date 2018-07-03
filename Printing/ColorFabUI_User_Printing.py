import bpy
from bpy.props import (BoolProperty,
                       PointerProperty,
                       )
from bpy.types import (Panel,
                       Operator,
                       AddonPreferences,
                       PropertyGroup,
                       ) 
import os
import sys
import mathutils 
from math import radians 

class ColorFabUI(bpy.types.Panel):
    """Creates a Custom Panel to Load and Save File, and Voxelize Models"""

    bl_label = "ColorFab Printing Panel"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'

    def draw(self, context):
        layout = self.layout

        # Buttons to load different files: obj, stl
        row = layout.row()
        row.label(text="Load file")
        split = layout.split()
        col = split.column(align=True)
        col.operator("import_scene.obj")
        col.operator("import_mesh.stl")

        # Voxelize the model and assign colors selected by user to the shell 
        row = layout.row()
        row.label(text="Prepare for Print")
        split = layout.split()
        col = split.column(align=True)
        scene = context.scene 
        col.prop(scene, "RED", text="Red") 
        col.prop(scene, "BLUE", text="Blue") 
        col.prop(scene, "GREEN", text="Green") 
        col.prop(scene, "YELLOW", text="Yellow") 
        col.operator("object.process_for_print", text="Process for Print")
    
################################################### Prepared to Print ###################################################
basedir = os.path.dirname(bpy.data.filepath)
if basedir not in sys.path:
    sys.path.append(basedir)
from VoxelObj import VoxelObj
from export_obj_by_layer import export_obj_by_layer
from load_obj import load_obj
from export_obj import export_obj

class ProcessForPrintOperator(Operator): 
    bl_idname = "object.process_for_print"
    bl_label = "Voxelize and assign color" 
    bl_options = {'REGISTER', 'UNDO'} 
    
    def makeMaterial(self, name, diffuse, specular, alpha):
        mat = bpy.data.materials.new(name)
        mat.diffuse_color = diffuse
        mat.diffuse_shader = 'LAMBERT'
        mat.diffuse_intensity = 1.0
        mat.specular_color = specular
        mat.specular_shader = 'COOKTORR'
        mat.specular_intensity = 0.5
        mat.alpha = alpha
        mat.ambient = 1
        return mat
    
    @classmethod 
    def poll(self, context): 
        return context.active_object is not None and context.active_object.type == 'MESH'
    
    def execute(self, context): 
        ## Voxelize with octree_depth = 8 
        ob = context.active_object
        mod = ob.modifiers.new("", 'REMESH')
        mod.octree_depth = 8
        mod.mode = 'BLOCKS'
        mod.scale = 0.99
        mod.use_remove_disconnected = True
        bpy.ops.object.modifier_apply(apply_as='DATA', modifier=mod.name)
        
        ## Export as OBJ 
        ob.select = True 
        bpy.ops.export_scene.obj(filepath = os.path.join(basedir, 'user.obj'), use_selection = True) 
        
        ## Assign Colors 
        filename = os.path.join(basedir, 'user.obj')
        voxelizeModel = load_obj(filename)
        color = 0 
        scene = context.scene 
        if(scene.RED == True): 
            color += 1
        if(scene.BLUE == True): 
            color += 1
        if(scene.GREEN == True): 
            color += 1
        if(scene.YELLOW == True): 
            color += 1
        assert color > 0, "Must select at least one color!" 
        
        blockSize = 6
        depth = 2
        export_obj(voxelizeModel, color, blockSize, depth)
        
        ## convert to STL 
        path = 'C:\Program Files\Blender Foundation\Blender'
        for root, dirs, files in os.walk(path):
            for f in files:
                if f.endswith('.obj'):
                    obj_file = os.path.join(path, f)
                    stl_file = os.path.splitext(obj_file)[0] + ".stl"
                    bpy.ops.object.select_all(action='SELECT')
                    bpy.ops.object.delete()
                    bpy.ops.import_scene.obj(filepath=obj_file)
                    bpy.ops.object.select_all(action='SELECT')
                    bpy.ops.export_mesh.stl(filepath=stl_file)
                    
        ## load and show color 
        objects_on_layer0 = [ob for ob in bpy.context.scene.objects if ob.layers[0]]
        for obj in objects_on_layer0:
            obj.select = True
            bpy.ops.object.delete()
            
        folder_path = "C:/Program Files/Blender Foundation/Blender/"
        models = ["material1.obj", "material2.obj", "material3.obj", "material4.obj", "material5.obj"] 
        for i in range(color+1): 
            bpy.context.scene.layers[i] = False  
            bpy.context.scene.layers[i+1] = True 
            path = os.path.join(folder_path, models[i]) 
            bpy.ops.import_scene.obj(filepath=path) 
        bpy.context.scene.layers[color+1] = False  
        
        red = self.makeMaterial('red', (1, 0, 0), (1, 1, 1), 1)
        green = self.makeMaterial('green', (0, 1, 0), (0.5, 0.5, 0), 1)
        blue = self.makeMaterial('blue', (0, 0, 1), (0.5, 0.5, 0), 1)
        yellow = self.makeMaterial('yellow', (1, 1, 0), (1, 1, 1), 1)
        white = self.makeMaterial('white', (1, 1, 1), (1, 1, 1), 1)
        bpy.context.scene.layers[0] = False  
        palette = [red, blue, green, yellow] 
        selection = [scene.RED, scene.BLUE, scene.GREEN, scene.YELLOW] 
        selection_index = 0 
        for i in range(1, color+1):
            objects_on_current_layer = [ob for ob in bpy.context.scene.objects if ob.layers[i]]
            while not selection[selection_index]: 
                selection_index += 1 
            for obj in objects_on_current_layer:
                context = 'MATERIAL' 
                obj.data.materials[0] = palette[selection_index] 
            selection_index += 1 
        
        object_inside = [ob for ob in bpy.context.scene.objects if ob.layers[color+1]]   
        for obj in object_inside: 
            context = 'MATERIAL' 
            obj.data.materials[0] = white
                
        return {'FINISHED'}
                    


def register():
    bpy.types.Scene.RED = bpy.props.BoolProperty(
        name="Red",
        description="Red selected or not",
        default = True )
    bpy.types.Scene.BLUE = bpy.props.BoolProperty(
        name="Blue",
        description="Blue selected or not",
        default = True)
    bpy.types.Scene.GREEN = bpy.props.BoolProperty(
        name="Green",
        description="Green selected or not",
        default = True)
    bpy.types.Scene.YELLOW = bpy.props.BoolProperty(
        name="Yellow",
        description="Yellow selected or not",
        default = True) 
    bpy.utils.register_class(ProcessForPrintOperator)
    bpy.utils.register_class(ColorFabUI)

def unregister():
    del bpy.types.Scene.red
    del bpy.types.Scene.blue
    del bpy.types.Scene.green
    del bpy.types.Scene.yellow
    bpy.utils.unregister_class(ProcessForPrintOperator)
    bpy.utils.unregister_class(ColorFabUI)

if __name__ == "__main__":
    register()