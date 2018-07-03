import bpy 
from bpy.types import Operator

class ColorFabPanel(bpy.types.Panel):
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
        col = split.column(align = True)
        
        col.operator("import_scene.obj")
        col.operator("import_mesh.stl")
        col.operator("import_scene.fbx")
        
        # Buttons to save blender file or export as stl file 
        row = layout.row()
        row.label(text="Save file")
        split = layout.split()
        col = split.column(align = True)
        
        col.operator("wm.save_as_mainfile")
        col.operator("wm.save_mainfile")
        col.operator("export_mesh.stl")
        col.operator("export_scene.obj")
        
        # Buttons to set voxelize parameters and process voxelization 
        row = layout.row()
        row.label(text="Voxelize Model")
        split = layout.split()
        col = split.column(align = True)
        col.prop(context.object, "set_octree_depth", text = "Resolution") 
        col.operator("object.voxel_preview_operator", text = "Preview")
        col.operator("object.voxel_operator", text = "Process")

        ## Buttons to import voxel models to different layers 
        #row = layout.row()
        #row.label(text="Import Objects to Layers") 
        #split = layout.split()
        #col = split.column(align = True) 
        #col.operator("object.import_layer_operator", text = "Layer One").layer = 0 
        #col.operator("object.import_layer_operator", text = "Layer Two").layer = 1 
        #col.operator("object.import_layer_operator", text = "Layer Three").layer = 2 
        
        # Buttons to assign colors to voxelized model
        row = layout.row()
        row.label(text="Assign Colors")
        split = layout.split()
        col = split.column(align = True)
        col.prop(context.object, "filename", text = "Enter OBJ File Name")
        col.prop(context.object, "set_colors", text = "Number of Colors")
        col.prop(context.object, "block_size", text="Voxel Size for Each Block")
        col.operator("object.assign_color_operator", text = "Process")
        
        # Button to load processed models to separate layers 
        row = layout.row()
        row.label(text="Convert Models to STL") 
        col = layout.split().column(align = True) 
        col.operator("object.convert_models_operator", text = "Convert All") 
        
        # Buttons to paint models 
        row = layout.row()
        row.label(text = "Paint") 
        col = layout.split().column(align = True) 
        col.operator("object.painting_operator", text = "Red").color = "red"
        col.operator("object.painting_operator", text = "Blue").color = "blue"
        
        # Buttons to convert color to black and white 
        row = layout.row()
        row.label(text="Process Models For Projection") 
        col = layout.split().column(align=True) 
        col.operator("object.convert_to_bw_operator", text = "Process Model for Projection") 
        
def register():
    bpy.utils.register_class(ColorFabPanel)



def unregister():
    bpy.utils.unregister_class(ColorFabPanel)
    

if __name__ == "__main__":
    register()