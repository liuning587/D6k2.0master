#ifndef MB_TREE_ITEM_DEFINE_H
#define MB_TREE_ITEM_DEFINE_H

// �ܹ���32 λ��ʾ
// ��� 2λ ��ʾ ������

 
enum DS_TREEITEM
{
	TREE_LEVEL_DEF = Qt::UserRole + 100,
	TREE_PLUGIN_ROOT = 100,  //<! ����ĸ��˵�

	TREE_PLUGIN_1 = 0x01000000,
	TREE_PLUGIN_1_LEAF1_M1 = 0x01010000,  //!< ���1��1���Ӳ˵�1

	TREE_PLUGIN_1_LEAF1_M2 = 0x01020000,  //!< ���1��1���Ӳ˵�2
	TREE_PLUGIN_1_LEAF1_M3 = 0x01030000,  //!< ���1��1���Ӳ˵�3
	TREE_PLUGIN_1_LEAF1_M4 = 0x01040000,  //!< ���1��1���Ӳ˵�4


	//	TREE_PLUGIN_CHILD_2  = 300,
	//	TREE_PLUGIN_CHILD_3  = 400,


};
 

//������ṹ 
enum OSC_DS_TREEDEF
{
	// 
	TI_COIL = TREE_PLUGIN_1_LEAF1_M1,
	TI_INPUT = TREE_PLUGIN_1_LEAF1_M2,
	TI_HR = TREE_PLUGIN_1_LEAF1_M3,
	TI_IR = TREE_PLUGIN_1_LEAF1_M4,

	 
};




 



#endif
