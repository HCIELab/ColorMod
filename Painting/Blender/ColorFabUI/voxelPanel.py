import bpy 
from bpy.types import Operator

class FilePanel(bpy.types.Panel):
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
        
        col.operator("imort_scene.obj")
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
        
        # Buttons to set voxelize parameters and process voxelization 
        row = layout.row()
        row.label(text="Voxelize Model")
        split = layout.split()
        col = split.column(align = True)
        col.prop(context.object, "set_octree_depth")
        col.operator("object.voxel_preview_operator" , text = "Preview")
        col.operator("object.voxel_operator" , text = "Process")

        # Buttons to import voxel models to different layers 
        row = layout.row()
        row.label(text="Import Objects to Layers") 
        split = layout.split()
        col = split.colum(align = True) 
        col.operator("object.import_layer_operator", text = "Layer One") 
    
def register():
    bpy.utils.register_class(FilePanel)



def unregister():
    bpy.utils.unregister_class(FilePanel)
    

if __name__ == "__main__":
    register()