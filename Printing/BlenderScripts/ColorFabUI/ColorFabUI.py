import bpy
from bpy.types import Operator
import os
import sys


class ColorFabUI(bpy.types.Panel):
    """Creates a Custom Panel to Load and Save File, and Voxelize Models"""

    bl_label = "ColorFab Panel"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'

    def draw(self, context):
        layout = self.layout

        # Buttons to load different files: obj, stl, fbx
        row = layout.row()
        row.label(text="Load file")
        split = layout.split()
        col = split.column(align=True)

        col.operator("import_scene.obj")
        col.operator("import_mesh.stl")
        col.operator("import_scene.fbx")

        # Buttons to save blender file or export as stl file
        row = layout.row()
        row.label(text="Save file")
        split = layout.split()
        col = split.column(align=True)
        col.operator("wm.save_as_mainfile")
        col.operator("wm.save_mainfile")
        col.operator("export_mesh.stl")
        col.operator("export_scene.obj")

        # Buttons to set voxelize parameters and process voxelization
        row = layout.row()
        row.label(text="Voxelize Model")
        split = layout.split()
        col = split.column(align=True)
        col.prop(context.object, "set_octree_depth", text="Resolution")
        col.operator("object.voxel_preview_operator", text="Preview")
        col.operator("object.voxel_operator", text="Process")

        # Buttons to assign colors to voxelized model
        row = layout.row()
        row.label(text="Assign Colors")
        split = layout.split()
        col = split.column(align = True)
        col.prop(context.object, "filename", text = "Enter OBJ File Name")
        col.prop(context.object, "set_colors", text = "Number of Colors")
        col.prop(context.object, "block_size", text="Voxel Size for Each Block")
        col.prop(context.object, "block_depth", text="Depth for Each Block")
        col.operator("object.assign_color_operator", text = "Process")

        ## Buttons to import voxel models to different layers
        # row = layout.row()
        # row.label(text="Import Objects to Layers")
        # split = layout.split()
        # col = split.column(align = True)
        # col.operator("object.import_layer_operator", text = "Layer One").layer = 0
        # col.operator("object.import_layer_operator", text = "Layer Two").layer = 1
        # col.operator("object.import_layer_operator", text = "Layer Three").layer = 2

        # Button to convert processed models to stl
        row = layout.row()
        row.label(text="Convert Models to STL")
        col = layout.split().column(align=True)
        col.operator("object.convert_models_operator", text="Convert All")

        # Buttons to paint models
        row = layout.row()
        row.label(text="Paint")
        col = layout.split().column(align=True)
        col.operator("object.prepare_colors_operator", text = "Prepare Colors for Painting")
        col.operator("object.painting_operator", text="Red").color = "red"
        col.operator("object.painting_operator", text="Blue").color = "blue"

        # Buttons to convert color to black and white
        row = layout.row()
        row.label(text="Process Models For Projection")
        col = layout.split().column(align=True)
        col.operator("object.convert_to_bw_operator", text="Process Model for Projection")

################################################### Voxelize Process ###################################################
class VoxelPreviewOperator(Operator):
    bl_idname = "object.voxel_preview_operator"
    bl_label = "Add Remesh modifier and apply"
    bl_options = {'REGISTER', 'UNDO'}

    @classmethod
    def poll(self, context):
        return True

    def execute(self, context):
        ob = context.active_object
        if "Remesh" in ob.modifiers:
            bpy.ops.object.modifier_remove(modifier="Remesh")
        bpy.ops.object.modifier_add(type='REMESH')
        mod = ob.modifiers["Remesh"]
        mod.octree_depth = context.object.set_octree_depth
        mod.mode = 'BLOCKS'
        mod.scale = 0.99
        mod.use_remove_disconnected = True
        return {'FINISHED'}


class VoxelOperator(Operator):
    bl_idname = "object.voxel_operator"
    bl_label = "Add Remesh modifier and apply"
    bl_options = {'REGISTER', 'UNDO'}

    @classmethod
    def poll(self, context):
        return context.active_object is not None and context.active_object.type == 'MESH'

    def execute(self, context):
        ob = context.active_object
        mod = ob.modifiers.new("", 'REMESH')
        mod.octree_depth = context.object.set_octree_depth
        mod.mode = 'BLOCKS'
        mod.scale = 0.99
        mod.use_remove_disconnected = True
        bpy.ops.object.modifier_apply(apply_as='DATA', modifier=mod.name)
        return {'FINISHED'}

################################################### Assigning Colors ###################################################
basedir = os.path.dirname(bpy.data.filepath)
if basedir not in sys.path:
    sys.path.append(basedir)
from VoxelObj import VoxelObj
from export_obj_by_layer import export_obj_by_layer
from load_obj import load_obj
from export_obj import export_obj

class ColorOperator(Operator):
    bl_idname = "object.assign_color_operator"
    bl_label = "Assign Colors to Shell and Separate Model by Colors"
    bl_options = {'REGISTER', 'UNDO'}

    @classmethod
    def poll(self, context):
        return True

    def execute(self, context):
        OBJFile = context.object.filename
        filename = os.path.join(basedir, OBJFile)
        voxelizeModel = load_obj(filename)
        color = context.object.set_colors
        blockSize = context.object.block_size
        depth = context.object.block_depth
        export_obj(voxelizeModel, color, blockSize, depth)
        return {'FINISHED'}

################################################### Convert to STL #####################################################
class ConvertModelsOperator(Operator):
    bl_idname = "object.convert_models_operator"
    bl_label = "Convert Processed OBJ Files to STL"
    bl_options = {'REGISTER', 'UNDO'}

    @classmethod
    def poll(self, context):
        return True

    def execute(self, context):
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
                    # for i in range(1,context.object.set_colors+1):
                    # context.scene.layers[i] = True
                    # bpy.ops.object.select_all(action='TOGGLE')
                    # bpy.ops.import_scene.obj(filepath=objpath)
                    # ob = context.scene.layers[i].active_object
                    # ob.layers[i] = True
                    # ob.layers = [ j==i for j in range(len(ob.layers)) ]
        return {'FINISHED'}

##############################################Preparing Colors for Painting#############################################
class PrepareColorsOperator(Operator):
    bl_idname = "object.prepare_colors_operator"
    bl_label = "Prepare colors for painting process"
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
        red = self.makeMaterial('red', (1, 0, 0), (1, 1, 1), 1)
        blue = self.makeMaterial('blue', (0, 0, 1), (0.5, 0.5, 0), 1)
        black = self.makeMaterial('black', (0, 0, 0), (1, 1, 1), 1)
        white = self.makeMaterial('white', (1, 1, 1), (1, 1, 1), 1)
        for i in range(1, 4):
            objects_on_current_layer = [ob for ob in bpy.context.scene.objects if ob.layers[i]]
            for obj in objects_on_current_layer:
                context = 'MATERIAL' 
                obj.data.materials.append(white) 

################################################### Painting Process ###################################################
class PaintingOperator(Operator):
    bl_idname = "object.painting_operator"
    bl_label = "Apply color to selected voxels"
    bl_options = {'REGISTER', 'UNDO'}

    color = bpy.props.StringProperty()

    @classmethod
    def poll(self, context):
        return context.active_object is not None

    def execute(self, context):
        obj = context.active_object
        if self.color == "red":
            obj.data.materials[0] = bpy.data.materials['red']
        elif self.color == "blue":
            obj.data.materials[0] = bpy.data.materials['blue']
        return {'FINISHED'}

########################################### Convert to BW for Projection ###############################################
class ConvertToBWOperator(Operator):
    bl_idname = "object.convert_to_bw_operator"
    bl_label = "Convert Colored Voxels to Black"
    bl_options = {'REGISTER', 'UNDO'}

    @classmethod
    def poll(self, context):
        return True

    def execute(self, context):
        color = ['white', 'red', 'blue']
        for i in range(1, len(color) + 1):
            objects_on_current_layer = [ob for ob in bpy.context.scene.objects if ob.layers[i]]
            for obj in objects_on_current_layer:
                if obj.data.materials != None:
                    if obj.data.materials[0] == bpy.data.materials[color[i]]:
                        obj.data.materials[0] = bpy.data.materials['black']
                    else:
                        obj.data.materials[0] = bpy.data.materials['white']
                else:
                    obj.data.materials.append('white')
        return {'FINISHED'}

def register():
    bpy.utils.register_class(VoxelOperator)
    bpy.utils.register_class(VoxelPreviewOperator)
    bpy.types.Object.set_octree_depth = bpy.props.IntProperty(
        name="Octree Depth",
        description="Voxelize the model with selected resolution",
        default=5,
        min=1,
        max=12
    )
    bpy.utils.register_class(ColorOperator)
    bpy.types.Object.set_colors = bpy.props.IntProperty(
        name="Number of Colors",
        description="Number of colors to be assigned to shell",
        default=2,
        min=1,
        max=4
    )
    bpy.types.Object.block_size = bpy.props.IntProperty(
        name="Number of Voxels",
        description="Number of voxels to be assigned to each color block",
        default=3,
        min=1,
        max=10
    )
    bpy.types.Object.block_depth = bpy.props.IntProperty(
        name="Depth of Color Blocks",
        description="Depth of Colored Layer",
        default=2,
        min=1,
        max=5
    )
    bpy.types.Object.filename = bpy.props.StringProperty(
        name="Name of OBJ File",
        description="Specify the obj file of the voxelized model"
    )
    bpy.utils.register_class(ConvertModelsOperator)
    bpy.utils.register_class(PrepareColorsOperator) 
    bpy.utils.register_class(PaintingOperator)
    bpy.utils.register_class(ConvertToBWOperator)
    bpy.utils.register_class(ColorFabUI)




def unregister():
    bpy.utils.unregister_class(VoxelOperator)
    bpy.utils.unregister_class(VoxelPreviewOperator)
    del bpy.types.Object.set_octree_depth
    bpy.utils.unregister_class(ColorOperator)
    del bpy.types.Object.set_colors
    del bpy.types.Object.block_size
    del bpy.types.Object.block_depth
    del bpy.types.Object.filename
    bpy.utils.unregister_class(ConvertModelsOperator)
    bpy.utils.unregister_class(PrepareColorsOperator) 
    bpy.utils.unregister_class(PaintingOperator)
    bpy.utils.unregister_class(ConvertToBWOperator)
    bpy.utils.unregister_class(ColorFabUI)


if __name__ == "__main__":
    register()