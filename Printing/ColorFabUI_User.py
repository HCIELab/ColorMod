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
from os import listdir
from os.path import isfile, join
import sys
import time
import mathutils 
from math import radians 
basedir = os.path.dirname(bpy.data.filepath)
if basedir not in sys.path:
    sys.path.append(basedir)
from VoxelObj import VoxelObj
from export_obj_by_layer import export_obj_by_layer
from load_obj import load_obj
from export_obj import export_obj
import userColorSelection
import projection 
import panelClick 
################################################### Printing Panel ###################################################
class ColorFabPrinting(bpy.types.Panel):
    """Creates a Custom Panel to Load Model and Convert it for ColorFab"""

    bl_label = "ColorFab Printing Panel"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'

    def draw(self, context):
        layout = self.layout
        bpy.context.window.cursor_set('DEFAULT')

        # Buttons to load different files: obj, stl
        row = layout.row()
        row.label(text="Load file")
        split = layout.split()
        col = split.column(align=True)
        col.operator("import_scene.obj")
        col.operator("import_mesh.stl")

        # Voxelize the model and assign colors selected by user to the shell 
        row = layout.row()
        row.label(text="Prepare for Printing")
        split = layout.split()
        col = split.column(align=True)
        scene = context.scene 
        col.prop(scene, "RED", text="Red") 
        col.prop(scene, "BLUE", text="Blue") 
        col.prop(scene, "YELLOW", text="Yellow") 
        col.prop(scene, "GREEN", text="Green") 
       
        row = layout.row()
        split = layout.split()
        col = split.column(align=True)
        col.operator("object.process_for_print", text="Process for Print")

################################################### Painting Panel ###################################################
class ColorFabPainting(bpy.types.Panel):
    """Creates a Custom Panel to Paint Model"""

    bl_label = "ColorFab Painting Panel"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'

    def draw(self, context):
        #bpy.context.window.cursor_set('PAINT_BRUSH')
        layout = self.layout

        # Buttons to load different files: obj, stl
        row = layout.row()
        row.label(text="Load Model for Painting")
        split = layout.split()
        col = split.column(align=True)
        #col.operator("import_mesh.stl", text="Import Printed Model")
        col.operator("object.prepare_for_painting", text="Import Printed Model")
        
        # Voxelize the model and assign colors selected by user to the shell 
        row = layout.row()
        row.label(text="Painting")
        split = layout.split()
        col = split.column(align=True)
        col.operator("object.red_paint", text="Red")
        col.operator("object.blue_paint", text="Blue")
        col.operator("object.yellow_paint", text="Yellow")
        col.operator("object.green_paint", text="Green")
        
        
        # Button to convert model to black and white for projection 
        row = layout.row()
        split = layout.split()
        col = split.column(align=True)
        col.operator("object.convert_to_bw_operator", text="Finalize Painting")
        
        row = layout.row()
        row.label(text="Projecting")
        split = layout.split()
        col = split.column(align=True)
        col.operator("object.projection_setup_operator", text="Projection Setup")
        

################################################### Prepared to Print ###################################################


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
        return panelClick.printing
        ## return context.active_object is not None and context.active_object.type == 'MESH'
    
    def execute(self, context): 
        ## Voxelize with octree_depth = 8 
        ob = context.active_object
        mod = ob.modifiers.new("", 'REMESH')
        mod.octree_depth = 6
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
        color = []
        scene = context.scene 
        if(scene.RED == True): 
            color.append('red') 
        if(scene.BLUE == True): 
            color.append('blue') 
        if(scene.GREEN == True): 
            color.append('green') 
        if(scene.YELLOW == True): 
            color.append('yellow') 
        assert len(color) > 0, "Must select at least one color!" 
        
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
        for i in range(len(color)): 
            bpy.context.scene.layers[i] = False  
            bpy.context.scene.layers[i+1] = True 
            filename = ''.join([color[i], '.obj']) 
            path = os.path.join(folder_path, filename) 
            bpy.ops.import_scene.obj(filepath=path) 
        
        bpy.context.scene.layers[len(color)] = False  
        bpy.context.scene.layers[len(color)+1] = True 
        filename = 'white.obj' 
        path = os.path.join(folder_path, filename)   
        bpy.ops.import_scene.obj(filepath=path) 
        bpy.context.scene.layers[len(color)+1] = False 
        
        
        red = self.makeMaterial('red', (1, 0, 0), (1, 1, 1), 1)
        green = self.makeMaterial('green', (0, 1, 0), (0.5, 0.5, 0), 1)
        blue = self.makeMaterial('blue', (0, 0, 1), (0.5, 0.5, 0), 1)
        yellow = self.makeMaterial('yellow', (1, 1, 0), (1, 1, 1), 1)
        white = self.makeMaterial('white', (1, 1, 1), (1, 1, 1), 1)
        bpy.context.scene.layers[0] = False  
        palette = [red, blue, green, yellow] 
        selection = [scene.RED, scene.BLUE, scene.GREEN, scene.YELLOW] 
        selection_index = 0 
        for i in range(1, len(color)+1):
            objects_on_current_layer = [ob for ob in bpy.context.scene.objects if ob.layers[i]]
            if color[i-1] == 'red': 
                addmaterial = red 
            elif color[i-1] == 'blue': 
                 addmaterial = blue
            elif color[i-1] == 'green': 
                 addmaterial = green
            elif color[i-1] == 'yellow': 
                 addmaterial = yellow
            obj_outside = [ob for ob in bpy.context.scene.objects if ob.layers[i]]
            for obj in obj_outside: 
                context = 'MATERIAL' 
                obj.data.materials[0] = addmaterial
        
        object_inside = [ob for ob in bpy.context.scene.objects if ob.layers[len(color)+1]]   
        for obj in object_inside: 
            context = 'MATERIAL' 
            obj.data.materials[0] = white
                
        return {'FINISHED'}

################################################### Prepared to Paint ###################################################                 
class PrepareforPaintingOperator(Operator): 
    bl_idname = "object.prepare_for_painting"
    bl_label = "Load Back Model and Prepare Color" 
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
        return True 
    
    def execute(self, context): 
        panelClick.printing = False 
        panelClick.painting = True 
        
        bpy.context.window.cursor_set('PAINT_BRUSH')
        userColorSelection.userColors  = []
        
        ## prepare color and reload models 
        red = self.makeMaterial('red', (1, 0, 0), (1, 1, 1), 1)
        green = self.makeMaterial('green', (0, 1, 0), (0.5, 0.5, 0), 1)
        blue = self.makeMaterial('blue', (0, 0, 1), (0.5, 0.5, 0), 1)
        yellow = self.makeMaterial('yellow', (1, 1, 0), (1, 1, 1), 1)
        white = self.makeMaterial('white', (1, 1, 1), (1, 1, 1), 1)
        black = self.makeMaterial('black', (0, 0, 0), (1, 1, 1), 1)
        
        objects_on_layer0 = [ob for ob in bpy.context.scene.objects if ob.layers[0]]
        for obj in objects_on_layer0:
            obj.select = True
            bpy.ops.object.delete()
        
        models = ['red.obj', 'blue.obj', 'green.obj', 'yellow.obj', 'white.obj'] 
        color = ['red', 'blue', 'green', 'yellow', 'white'] 
        path = 'C:\Program Files\Blender Foundation\Blender'
        allFiles = [f for f in listdir(path) if isfile(join(path, f))]
        print(allFiles) 
        layer = 0 
        for i in range(5): 
            if models[i] in allFiles:
                bpy.context.scene.layers[layer] = False
                bpy.context.scene.layers[layer+1] = True 
                layer += 1 
                userColorSelection.userColors.append(color[i])
                modelPath = os.path.join(path, models[i]) 
                bpy.ops.import_scene.obj(filepath=modelPath) 
                objects_on_current_layer = [ob for ob in bpy.context.scene.objects if ob.layers[layer]]
                for obj in objects_on_current_layer: 
                    context = 'MATERIAL' 
                    obj.data.materials[0] = white 
                obj = objects_on_current_layer[0]
                obj.select = True 
                bpy.ops.mesh.separate(type = 'LOOSE')
        bpy.context.scene.layers[layer+1] = False 
        
        return {'FINISHED'} 
        
################################################### Painting Process ###################################################
class RedOperator(Operator):
    bl_idname = "object.red_paint"
    bl_label = "Apply red to selected voxels"
    bl_options = {'REGISTER', 'UNDO'}

    @classmethod
    def poll(self, context):
        return panelClick.painting and 'red' in userColorSelection.userColors 

    def execute(self, context):
        #if 'red' not in userColorSelection.userColors: 
        #    return {'CANCELLED'} 
        obj = context.active_object
        obj.data.materials[0] = bpy.data.materials['red']
        return {'FINISHED'}
    
class BlueOperator(Operator):
    bl_idname = "object.blue_paint"
    bl_label = "Apply blue to selected voxels"
    bl_options = {'REGISTER', 'UNDO'}

    @classmethod
    def poll(self, context):
        return panelClick.painting and 'blue' in userColorSelection.userColors 

    def execute(self, context):
        #if 'blue' not in userColorSelection.userColors: 
        #    return {'CANCELLED'} 
        obj = context.active_object
        obj.data.materials[0] = bpy.data.materials['blue']
        return {'FINISHED'}
    
class GreenOperator(Operator):
    bl_idname = "object.green_paint"
    bl_label = "Apply green to selected voxels"
    bl_options = {'REGISTER', 'UNDO'}

    @classmethod
    def poll(self, context):
        return panelClick.painting and 'green' in userColorSelection.userColors 

    def execute(self, context):
        #if 'green' not in userColorSelection.userColors: 
        #    return {'CANCELLED'} 
        obj = context.active_object
        obj.data.materials[0] = bpy.data.materials['green']
        return {'FINISHED'}
    
class YellowOperator(Operator):
    bl_idname = "object.yellow_paint"
    bl_label = "Apply yellow to selected voxels"
    bl_options = {'REGISTER', 'UNDO'}

    @classmethod
    def poll(self, context):
        return panelClick.painting and 'yellow' in userColorSelection.userColors 
 

    def execute(self, context):
        #if 'red' not in userColorSelection.userColors: 
        #    return {'CANCELLED'} 
        obj = context.active_object
        obj.data.materials[0] = bpy.data.materials['yellow']
        return {'FINISHED'}

########################################### Convert to BW for Projection ###############################################
class ConvertToBWOperator(Operator):
    bl_idname = "object.convert_to_bw_operator"
    bl_label = "Convert Colored Voxels to Black"
    bl_options = {'REGISTER', 'UNDO'}

    @classmethod
    def poll(self, context):
        return False 

    def execute(self, context):
        palette = ['white', 'red', 'blue', 'green', 'yellow']
        for i in range(1, len(palette)):
            objects_on_current_layer = [ob for ob in bpy.context.scene.objects if ob.layers[i]]
            for obj in objects_on_current_layer:
                if obj.data.materials != None:
                    if obj.data.materials[0] == bpy.data.materials[palette[i]]:
                        obj.data.materials[0] = bpy.data.materials['black']
                    else:
                        obj.data.materials[0] = bpy.data.materials['white']
                else:
                    obj.data.materials.append('white')
        
        # join everything together 
        bpy.context.scene.layers[0] = True 
        bpy.ops.object.select_all(action='SELECT') 
        bpy.ops.object.join() 
        bpy.ops.object.origin_set(type='ORIGIN_GEOMETRY') 
        obj = bpy.context.scene.objects.active
        obj.location.x = 11.06
        obj.location.y = -11.06
        obj.location.z = 2.04
        
        projection.projectionModule = True 
        
        return {'FINISHED'}

########################################### Set Up Projection ###############################################
class ProjectionSetupOperator(Operator): 
    bl_idname = "object.projection_setup_operator" 
    bl_label = "Set Up Projection" 
    bl_options = {'REGISTER', 'UNDO'}
    
    @classmethod
    def poll(self, context): 
        return projection.projectionModule
    
    def execute(self, context): 
        
        bpy.ops.object.camera_add()
        scene = bpy.data.scenes["Scene"]
        
        # Set camera rotation in euler angles
        scene.camera.rotation_mode = 'XYZ'
        scene.camera.rotation_euler[0] = 0 
        scene.camera.rotation_euler[1] = 0
        scene.camera.rotation_euler[2] = radians(90)
        
        # Set camera location 
        scene.camera.location.x = 11 
        scene.camera.location.y = -11
        scene.camera.location.z = 80
        
        bpy.ops.view3d.viewnumpad(type='CAMERA')
        bpy.ops.wm.window_duplicate()
        bpy.ops.screen.screen_full_area(use_hide_panels=True)
        #parenting camera to empty 
        #origin = (0,0,0) #can be replaced with b_obj.location
        #empty = bpy.ops.object.empty_add(type='PLAIN_AXES')
        #empty.location = origin
        #scene.camera.select = True 
        #empty.select = True 
        #bpy.context.scene.objects.active = empty 
        #bpy.ops.object.parent_set()
        
        bpy.ops.object.empty_add(type='PLAIN_AXES') 
        empty = bpy.context.scene.objects.active
        empty.location.x = 0 
        empty.location.y = 0 
        empty.location.z = 0 
        
        empty.select = True 
        scene = bpy.data.scenes["Scene"]
        scene.camera.select = True 
        bpy.ops.object.parent_set(type='OBJECT', keep_transform=True) 
        
        scene.camera.rotation_euler[0] = 0 
        scene.camera.rotation_euler[1] = 0
        scene.camera.rotation_euler[2] = radians(90)
        
        projection.projectionModule = False 
        
        return {'FINISHED'}
    
########################################### Register & Unregister ###############################################

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
    bpy.utils.register_class(PrepareforPaintingOperator)
    bpy.utils.register_class(RedOperator)
    bpy.utils.register_class(BlueOperator)
    bpy.utils.register_class(GreenOperator)
    bpy.utils.register_class(YellowOperator)
    bpy.utils.register_class(ConvertToBWOperator)
    bpy.utils.register_class(ProjectionSetupOperator)
    bpy.utils.register_class(ColorFabPrinting)
    bpy.utils.register_class(ColorFabPainting)

def unregister():
    del bpy.types.Scene.red
    del bpy.types.Scene.blue
    del bpy.types.Scene.green
    del bpy.types.Scene.yellow
    bpy.utils.unregister_class(ProcessForPrintOperator)
    bpy.utils.unregister_class(PrepareforPaintingOperator)
    bpy.utils.unregister_class(RedOperator)
    bpy.utils.unregister_class(BlueOperator)
    bpy.utils.unregister_class(GreenOperator)
    bpy.utils.unregister_class(YellowOperator)
    bpy.utils.unregister_class(ConvertToBWOperator)
    bpy.utils.unregister_class(ProjectionSetupOperator)
    bpy.utils.unregister_class(ColorFabPrinting)
    bpy.utils.unregister_class(ColorFabPainting)
    

if __name__ == "__main__":
    register()