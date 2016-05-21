constant int edge_table[256] = {
	0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
	0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
	0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
	0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
	0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
	0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
	0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
	0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
	0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
	0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
	0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
	0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
	0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
	0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
	0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
	0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
	0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
	0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
	0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
	0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
	0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
	0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
	0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
	0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
	0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
	0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
	0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
	0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
	0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
	0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
	0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
	0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0
};

constant int triangle_table[256][16] = {
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 8, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 8, 1, 1, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 2, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 8, 0, 10, 2, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 2, 9, 9, 2, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 8, 2, 8, 10, 2, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 11, 0, 0, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 9, 1, 11, 3, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 11, 1, 11, 9, 1, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{1, 10, 3, 3, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 10, 0, 10, 8, 0, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{0, 9, 3, 9, 11, 3, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{10, 8, 9, 11, 8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 3, 4, 4, 3, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 1, 0, 7, 4, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 1, 4, 1, 7, 4, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 2, 1, 7, 4, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 4, 3, 4, 0, 3, 10, 2, 1, -1, -1, -1, -1, -1, -1, -1},
	{10, 2, 9, 2, 0, 9, 7, 4, 8, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 2, 7, 9, 2, 3, 7, 2, 4, 9, 7, -1, -1, -1, -1},
	{7, 4, 8, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 4, 11, 4, 2, 11, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{1, 0, 9, 7, 4, 8, 11, 3, 2, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 4, 11, 4, 9, 2, 11, 9, 1, 2, 9, -1, -1, -1, -1},
	{1, 10, 3, 10, 11, 3, 4, 8, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 11, 1, 11, 4, 1, 4, 0, 1, 4, 11, 7, -1, -1, -1, -1},
	{8, 7, 4, 11, 0, 9, 10, 11, 9, 3, 0, 11, -1, -1, -1, -1},
	{11, 7, 4, 9, 11, 4, 10, 11, 9, -1, -1, -1, -1, -1, -1, -1},
	{4, 5, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 5, 9, 3, 8, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 5, 0, 0, 5, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 5, 8, 5, 3, 8, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{10, 2, 1, 4, 5, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 0, 3, 10, 2, 1, 5, 9, 4, -1, -1, -1, -1, -1, -1, -1},
	{10, 2, 5, 2, 4, 5, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 2, 5, 2, 3, 4, 5, 3, 8, 4, 3, -1, -1, -1, -1},
	{4, 5, 9, 11, 3, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 11, 0, 11, 8, 0, 5, 9, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 5, 0, 5, 1, 0, 11, 3, 2, -1, -1, -1, -1, -1, -1, -1},
	{5, 1, 2, 8, 5, 2, 11, 8, 2, 5, 8, 4, -1, -1, -1, -1},
	{11, 3, 10, 3, 1, 10, 4, 5, 9, -1, -1, -1, -1, -1, -1, -1},
	{5, 9, 4, 1, 8, 0, 1, 10, 8, 10, 11, 8, -1, -1, -1, -1},
	{0, 4, 5, 11, 0, 5, 10, 11, 5, 3, 0, 11, -1, -1, -1, -1},
	{8, 4, 5, 10, 8, 5, 11, 8, 10, -1, -1, -1, -1, -1, -1, -1},
	{8, 7, 9, 9, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 3, 9, 3, 5, 9, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{8, 7, 0, 7, 1, 0, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{3, 5, 1, 7, 5, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 7, 9, 7, 5, 9, 2, 1, 10, -1, -1, -1, -1, -1, -1, -1},
	{2, 1, 10, 0, 5, 9, 0, 3, 5, 3, 7, 5, -1, -1, -1, -1},
	{2, 0, 8, 5, 2, 8, 7, 5, 8, 2, 5, 10, -1, -1, -1, -1},
	{5, 10, 2, 3, 5, 2, 7, 5, 3, -1, -1, -1, -1, -1, -1, -1},
	{5, 9, 7, 9, 8, 7, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1},
	{7, 5, 9, 2, 7, 9, 0, 2, 9, 11, 7, 2, -1, -1, -1, -1},
	{11, 3, 2, 8, 1, 0, 8, 7, 1, 7, 5, 1, -1, -1, -1, -1},
	{1, 2, 11, 7, 1, 11, 5, 1, 7, -1, -1, -1, -1, -1, -1, -1},
	{8, 5, 9, 7, 5, 8, 3, 1, 10, 11, 3, 10, -1, -1, -1, -1},
	{0, 7, 5, 9, 0, 5, 0, 11, 7, 10, 0, 1, 0, 10, 11, -1},
	{0, 10, 11, 3, 0, 11, 0, 5, 10, 7, 0, 8, 0, 7, 5, -1},
	{5, 10, 11, 5, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 8, 0, 6, 10, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 0, 9, 6, 10, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 8, 1, 8, 9, 1, 6, 10, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 6, 1, 1, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{5, 6, 1, 6, 2, 1, 8, 0, 3, -1, -1, -1, -1, -1, -1, -1},
	{5, 6, 9, 6, 0, 9, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{8, 9, 5, 2, 8, 5, 6, 2, 5, 8, 2, 3, -1, -1, -1, -1},
	{11, 3, 2, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 0, 11, 0, 2, 11, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 1, 0, 11, 3, 2, 6, 10, 5, -1, -1, -1, -1, -1, -1, -1},
	{6, 10, 5, 2, 9, 1, 2, 11, 9, 11, 8, 9, -1, -1, -1, -1},
	{11, 3, 6, 3, 5, 6, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{11, 8, 0, 5, 11, 0, 1, 5, 0, 6, 11, 5, -1, -1, -1, -1},
	{6, 11, 3, 6, 3, 0, 5, 6, 0, 9, 5, 0, -1, -1, -1, -1},
	{9, 5, 6, 11, 9, 6, 8, 9, 11, -1, -1, -1, -1, -1, -1, -1},
	{6, 10, 5, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 3, 4, 3, 7, 4, 10, 5, 6, -1, -1, -1, -1, -1, -1, -1},
	{0, 9, 1, 6, 10, 5, 7, 4, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 6, 10, 7, 9, 1, 3, 7, 1, 4, 9, 7, -1, -1, -1, -1},
	{2, 1, 6, 1, 5, 6, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1},
	{5, 2, 1, 6, 2, 5, 4, 0, 3, 7, 4, 3, -1, -1, -1, -1},
	{7, 4, 8, 5, 0, 9, 5, 6, 0, 6, 2, 0, -1, -1, -1, -1},
	{9, 3, 7, 4, 9, 7, 9, 2, 3, 6, 9, 5, 9, 6, 2, -1},
	{2, 11, 3, 4, 8, 7, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1},
	{6, 10, 5, 2, 7, 4, 0, 2, 4, 11, 7, 2, -1, -1, -1, -1},
	{9, 1, 0, 8, 7, 4, 11, 3, 2, 6, 10, 5, -1, -1, -1, -1},
	{1, 2, 9, 2, 11, 9, 11, 4, 9, 4, 11, 7, 6, 10, 5, -1},
	{7, 4, 8, 5, 11, 3, 1, 5, 3, 6, 11, 5, -1, -1, -1, -1},
	{11, 1, 5, 6, 11, 5, 11, 0, 1, 4, 11, 7, 11, 4, 0, -1},
	{9, 5, 0, 5, 6, 0, 6, 3, 0, 3, 6, 11, 7, 4, 8, -1},
	{9, 5, 6, 11, 9, 6, 9, 7, 4, 9, 11, 7, -1, -1, -1, -1},
	{9, 4, 10, 10, 4, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{6, 10, 4, 10, 9, 4, 3, 8, 0, -1, -1, -1, -1, -1, -1, -1},
	{1, 0, 10, 0, 6, 10, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 8, 6, 1, 8, 4, 6, 8, 10, 1, 6, -1, -1, -1, -1},
	{9, 4, 1, 4, 2, 1, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{8, 0, 3, 9, 2, 1, 9, 4, 2, 4, 6, 2, -1, -1, -1, -1},
	{4, 2, 0, 6, 2, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 4, 2, 8, 6, 2, 4, -1, -1, -1, -1, -1, -1, -1},
	{9, 4, 10, 4, 6, 10, 3, 2, 11, -1, -1, -1, -1, -1, -1, -1},
	{2, 8, 0, 11, 8, 2, 10, 9, 4, 6, 10, 4, -1, -1, -1, -1},
	{2, 11, 3, 6, 1, 0, 4, 6, 0, 10, 1, 6, -1, -1, -1, -1},
	{1, 4, 6, 10, 1, 6, 1, 8, 4, 11, 1, 2, 1, 11, 8, -1},
	{4, 6, 9, 6, 3, 9, 3, 1, 9, 3, 6, 11, -1, -1, -1, -1},
	{1, 11, 8, 0, 1, 8, 1, 6, 11, 4, 1, 9, 1, 4, 6, -1},
	{6, 11, 3, 0, 6, 3, 4, 6, 0, -1, -1, -1, -1, -1, -1, -1},
	{8, 4, 6, 8, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{6, 10, 7, 10, 8, 7, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 7, 0, 7, 10, 0, 10, 9, 0, 10, 7, 6, -1, -1, -1, -1},
	{7, 6, 10, 7, 10, 1, 8, 7, 1, 0, 8, 1, -1, -1, -1, -1},
	{7, 6, 10, 1, 7, 10, 3, 7, 1, -1, -1, -1, -1, -1, -1, -1},
	{6, 2, 1, 8, 6, 1, 9, 8, 1, 7, 6, 8, -1, -1, -1, -1},
	{9, 6, 2, 1, 9, 2, 9, 7, 6, 3, 9, 0, 9, 3, 7, -1},
	{0, 8, 7, 6, 0, 7, 2, 0, 6, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 7, 2, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 3, 2, 8, 6, 10, 9, 8, 10, 7, 6, 8, -1, -1, -1, -1},
	{7, 0, 2, 11, 7, 2, 7, 9, 0, 10, 7, 6, 7, 10, 9, -1},
	{0, 8, 1, 8, 7, 1, 7, 10, 1, 10, 7, 6, 11, 3, 2, -1},
	{1, 2, 11, 7, 1, 11, 1, 6, 10, 1, 7, 6, -1, -1, -1, -1},
	{6, 9, 8, 7, 6, 8, 6, 1, 9, 3, 6, 11, 6, 3, 1, -1},
	{1, 9, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 7, 6, 0, 7, 0, 11, 3, 0, 6, 11, -1, -1, -1, -1},
	{6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 0, 3, 6, 7, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 1, 0, 6, 7, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 1, 8, 1, 3, 8, 6, 7, 11, -1, -1, -1, -1, -1, -1, -1},
	{2, 1, 10, 7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 2, 1, 8, 0, 3, 7, 11, 6, -1, -1, -1, -1, -1, -1, -1},
	{0, 9, 2, 9, 10, 2, 7, 11, 6, -1, -1, -1, -1, -1, -1, -1},
	{7, 11, 6, 3, 10, 2, 3, 8, 10, 8, 9, 10, -1, -1, -1, -1},
	{3, 2, 7, 7, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 0, 7, 0, 6, 7, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{6, 7, 2, 7, 3, 2, 9, 1, 0, -1, -1, -1, -1, -1, -1, -1},
	{2, 6, 1, 6, 8, 1, 8, 9, 1, 6, 7, 8, -1, -1, -1, -1},
	{6, 7, 10, 7, 1, 10, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{6, 7, 10, 10, 7, 1, 7, 8, 1, 8, 0, 1, -1, -1, -1, -1},
	{7, 3, 0, 10, 7, 0, 9, 10, 0, 7, 10, 6, -1, -1, -1, -1},
	{10, 6, 7, 8, 10, 7, 9, 10, 8, -1, -1, -1, -1, -1, -1, -1},
	{4, 8, 6, 6, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 6, 3, 6, 0, 3, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{11, 6, 8, 6, 4, 8, 1, 0, 9, -1, -1, -1, -1, -1, -1, -1},
	{6, 4, 9, 3, 6, 9, 1, 3, 9, 6, 3, 11, -1, -1, -1, -1},
	{4, 8, 6, 8, 11, 6, 1, 10, 2, -1, -1, -1, -1, -1, -1, -1},
	{10, 2, 1, 11, 0, 3, 11, 6, 0, 6, 4, 0, -1, -1, -1, -1},
	{8, 11, 4, 11, 6, 4, 9, 2, 0, 9, 10, 2, -1, -1, -1, -1},
	{3, 9, 10, 2, 3, 10, 3, 4, 9, 6, 3, 11, 3, 6, 4, -1},
	{3, 2, 8, 2, 4, 8, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{2, 4, 0, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 9, 1, 4, 3, 2, 6, 4, 2, 8, 3, 4, -1, -1, -1, -1},
	{4, 9, 1, 2, 4, 1, 6, 4, 2, -1, -1, -1, -1, -1, -1, -1},
	{3, 1, 8, 1, 6, 8, 6, 4, 8, 1, 10, 6, -1, -1, -1, -1},
	{0, 1, 10, 6, 0, 10, 4, 0, 6, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 4, 8, 3, 4, 3, 10, 6, 9, 3, 0, 3, 9, 10, -1},
	{4, 9, 10, 4, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{5, 9, 4, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 8, 0, 5, 9, 4, 6, 7, 11, -1, -1, -1, -1, -1, -1, -1},
	{1, 0, 5, 0, 4, 5, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1},
	{6, 7, 11, 4, 3, 8, 4, 5, 3, 5, 1, 3, -1, -1, -1, -1},
	{4, 5, 9, 2, 1, 10, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1},
	{7, 11, 6, 10, 2, 1, 3, 8, 0, 5, 9, 4, -1, -1, -1, -1},
	{11, 6, 7, 10, 4, 5, 10, 2, 4, 2, 0, 4, -1, -1, -1, -1},
	{8, 4, 3, 4, 5, 3, 5, 2, 3, 2, 5, 10, 6, 7, 11, -1},
	{3, 2, 7, 2, 6, 7, 9, 4, 5, -1, -1, -1, -1, -1, -1, -1},
	{4, 5, 9, 6, 8, 0, 2, 6, 0, 7, 8, 6, -1, -1, -1, -1},
	{2, 6, 3, 6, 7, 3, 0, 5, 1, 0, 4, 5, -1, -1, -1, -1},
	{8, 2, 6, 7, 8, 6, 8, 1, 2, 5, 8, 4, 8, 5, 1, -1},
	{4, 5, 9, 6, 1, 10, 6, 7, 1, 7, 3, 1, -1, -1, -1, -1},
	{10, 6, 1, 6, 7, 1, 7, 0, 1, 0, 7, 8, 4, 5, 9, -1},
	{10, 0, 4, 5, 10, 4, 10, 3, 0, 7, 10, 6, 10, 7, 3, -1},
	{10, 6, 7, 8, 10, 7, 10, 4, 5, 10, 8, 4, -1, -1, -1, -1},
	{5, 9, 6, 9, 11, 6, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{11, 6, 3, 3, 6, 0, 6, 5, 0, 5, 9, 0, -1, -1, -1, -1},
	{8, 11, 0, 11, 5, 0, 5, 1, 0, 11, 6, 5, -1, -1, -1, -1},
	{3, 11, 6, 5, 3, 6, 1, 3, 5, -1, -1, -1, -1, -1, -1, -1},
	{10, 2, 1, 11, 5, 9, 8, 11, 9, 6, 5, 11, -1, -1, -1, -1},
	{3, 11, 0, 11, 6, 0, 6, 9, 0, 9, 6, 5, 10, 2, 1, -1},
	{5, 8, 11, 6, 5, 11, 5, 0, 8, 2, 5, 10, 5, 2, 0, -1},
	{3, 11, 6, 5, 3, 6, 3, 10, 2, 3, 5, 10, -1, -1, -1, -1},
	{9, 8, 5, 8, 2, 5, 2, 6, 5, 2, 8, 3, -1, -1, -1, -1},
	{6, 5, 9, 0, 6, 9, 2, 6, 0, -1, -1, -1, -1, -1, -1, -1},
	{8, 5, 1, 0, 8, 1, 8, 6, 5, 2, 8, 3, 8, 2, 6, -1},
	{6, 5, 1, 6, 1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{6, 3, 1, 10, 6, 1, 6, 8, 3, 9, 6, 5, 6, 9, 8, -1},
	{0, 1, 10, 6, 0, 10, 0, 5, 9, 0, 6, 5, -1, -1, -1, -1},
	{8, 3, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{6, 5, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 5, 11, 11, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 5, 11, 5, 7, 11, 0, 3, 8, -1, -1, -1, -1, -1, -1, -1},
	{7, 11, 5, 11, 10, 5, 0, 9, 1, -1, -1, -1, -1, -1, -1, -1},
	{5, 7, 10, 7, 11, 10, 1, 8, 9, 1, 3, 8, -1, -1, -1, -1},
	{2, 1, 11, 1, 7, 11, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{3, 8, 0, 7, 2, 1, 5, 7, 1, 11, 2, 7, -1, -1, -1, -1},
	{5, 7, 9, 7, 2, 9, 2, 0, 9, 7, 11, 2, -1, -1, -1, -1},
	{2, 5, 7, 11, 2, 7, 2, 9, 5, 8, 2, 3, 2, 8, 9, -1},
	{10, 5, 2, 5, 3, 2, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 2, 8, 2, 5, 8, 5, 7, 8, 5, 2, 10, -1, -1, -1, -1},
	{1, 0, 9, 3, 10, 5, 7, 3, 5, 2, 10, 3, -1, -1, -1, -1},
	{2, 8, 9, 1, 2, 9, 2, 7, 8, 5, 2, 10, 2, 5, 7, -1},
	{5, 3, 1, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 8, 0, 1, 7, 0, 5, 7, 1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 5, 3, 9, 7, 3, 5, -1, -1, -1, -1, -1, -1, -1},
	{7, 8, 9, 7, 9, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 8, 5, 8, 10, 5, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{4, 0, 5, 0, 11, 5, 11, 10, 5, 0, 3, 11, -1, -1, -1, -1},
	{9, 1, 0, 10, 4, 8, 11, 10, 8, 5, 4, 10, -1, -1, -1, -1},
	{4, 11, 10, 5, 4, 10, 4, 3, 11, 1, 4, 9, 4, 1, 3, -1},
	{1, 5, 2, 5, 8, 2, 8, 11, 2, 8, 5, 4, -1, -1, -1, -1},
	{11, 4, 0, 3, 11, 0, 11, 5, 4, 1, 11, 2, 11, 1, 5, -1},
	{5, 2, 0, 9, 5, 0, 5, 11, 2, 8, 5, 4, 5, 8, 11, -1},
	{5, 4, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 5, 2, 2, 5, 3, 5, 4, 3, 4, 8, 3, -1, -1, -1, -1},
	{2, 10, 5, 4, 2, 5, 0, 2, 4, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 3, 10, 5, 3, 5, 8, 3, 8, 5, 4, 9, 1, 0, -1},
	{2, 10, 5, 4, 2, 5, 2, 9, 1, 2, 4, 9, -1, -1, -1, -1},
	{5, 4, 8, 3, 5, 8, 1, 5, 3, -1, -1, -1, -1, -1, -1, -1},
	{5, 4, 0, 5, 0, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{5, 4, 8, 3, 5, 8, 5, 0, 9, 5, 3, 0, -1, -1, -1, -1},
	{5, 4, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 11, 4, 11, 9, 4, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{3, 8, 0, 7, 9, 4, 7, 11, 9, 11, 10, 9, -1, -1, -1, -1},
	{11, 10, 1, 4, 11, 1, 0, 4, 1, 11, 4, 7, -1, -1, -1, -1},
	{4, 1, 3, 8, 4, 3, 4, 10, 1, 11, 4, 7, 4, 11, 10, -1},
	{7, 11, 4, 4, 11, 9, 11, 2, 9, 2, 1, 9, -1, -1, -1, -1},
	{4, 7, 9, 7, 11, 9, 11, 1, 9, 1, 11, 2, 3, 8, 0, -1},
	{4, 7, 11, 2, 4, 11, 0, 4, 2, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 11, 2, 4, 11, 4, 3, 8, 4, 2, 3, -1, -1, -1, -1},
	{10, 9, 2, 9, 7, 2, 7, 3, 2, 9, 4, 7, -1, -1, -1, -1},
	{7, 10, 9, 4, 7, 9, 7, 2, 10, 0, 7, 8, 7, 0, 2, -1},
	{10, 7, 3, 2, 10, 3, 10, 4, 7, 0, 10, 1, 10, 0, 4, -1},
	{2, 10, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 4, 7, 1, 4, 3, 1, 7, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 4, 7, 1, 4, 1, 8, 0, 1, 7, 8, -1, -1, -1, -1},
	{3, 0, 4, 3, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 8, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 10, 9, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 3, 11, 9, 3, 10, 9, 11, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 0, 8, 10, 0, 11, 10, 8, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 3, 10, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 2, 1, 9, 11, 1, 8, 11, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 3, 11, 9, 3, 9, 2, 1, 9, 11, 2, -1, -1, -1, -1},
	{11, 2, 0, 11, 0, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 2, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 2, 10, 8, 2, 9, 8, 10, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 9, 2, 9, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 2, 10, 8, 2, 8, 1, 0, 8, 10, 1, -1, -1, -1, -1},
	{2, 10, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 1, 8, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

struct basic_vertex {
	float3 position;
	float3 normal;
};

struct cell {
	float3 vertices[8];
	float values[8];
};

float value(global const float* volume, size_t resolution, size_t x, size_t y, size_t z) {
	size_t actual_resolution = resolution + 1;
	return volume[z + actual_resolution * (y + actual_resolution * x)];
}

float sample(global const float* volume, size_t resolution, float grid_length, float3 p, size_t sample_resolution) {
	size_t scale = resolution / sample_resolution;
	float sample_rate = grid_length / sample_resolution;

	size_t index_x = (size_t)(p.x / sample_rate);
	size_t index_y = (size_t)(p.y / sample_rate);
	size_t index_z = (size_t)(p.z / sample_rate);

	float x0 = index_x * sample_rate;
	float y0 = index_y * sample_rate;
	float z0 = index_z * sample_rate;
	float x1 = (index_x + 1) * sample_rate;
	if (p.x >= sample_resolution * sample_rate) {
		x1 = (index_x - 1) * sample_rate;
	} else if (p.x <= 0.f) {
		x1 = (index_x + 1) * sample_rate;
	}
	float y1 = (index_y + 1) * sample_rate;
	if (p.y >= sample_resolution * sample_rate) {
		y1 = (index_y - 1) * sample_rate;
	} else if (p.y <= 0.f) {
		y1 = (index_y + 1) * sample_rate;
	}
	float z1 = (index_z + 1) * sample_rate;
	if (p.z >= sample_resolution * sample_rate) {
		z1 = (index_z - 1) * sample_rate;
	} else if (p.x <= 0.f) {
		z1 = (index_z + 1) * sample_rate;
	}

	float xd = (p.x - x0) / (x1 - x0);
	float yd = (p.y - y0) / (y1 - y0);
	float zd = (p.z - z0) / (z1 - z0);

	float c00 = value(volume, resolution, (size_t)(x0 / sample_rate) * scale, (size_t)(y0 / sample_rate) * scale, (size_t)(z0 / sample_rate) * scale) * (1.f - xd)
		+ value(volume, resolution, (size_t)(x1 / sample_rate) * scale, (size_t)(y0 / sample_rate) * scale, (size_t)(z0 / sample_rate) * scale) * xd;
	float c01 = value(volume, resolution, (size_t)(x0 / sample_rate) * scale, (size_t)(y0 / sample_rate) * scale, (size_t)(z1 / sample_rate) * scale) * (1.f - xd)
		+ value(volume, resolution, (size_t)(x1 / sample_rate) * scale, (size_t)(y0 / sample_rate) * scale, (size_t)(z1 / sample_rate) * scale) * xd;
	float c10 = value(volume, resolution, (size_t)(x0 / sample_rate) * scale, (size_t)(y1 / sample_rate) * scale, (size_t)(z0 / sample_rate) * scale) * (1.f - xd)
		+ value(volume, resolution, (size_t)(x1 / sample_rate) * scale, (size_t)(y1 / sample_rate) * scale, (size_t)(z0 / sample_rate) * scale) * xd;
	float c11 = value(volume, resolution, (size_t)(x0 / sample_rate) * scale, (size_t)(y1 / sample_rate) * scale, (size_t)(z1 / sample_rate) * scale) * (1.f - xd)
		+ value(volume, resolution, (size_t)(x1 / sample_rate) * scale, (size_t)(y1 / sample_rate) * scale, (size_t)(z1 / sample_rate) * scale) * xd;

	float c0 = c00 * (1.f - yd) + c10 * yd;
	float c1 = c01 * (1.f - yd) + c11 * yd;

	return c0 * (1.f - zd) + c1 * zd;
}

float3 interpolate_vertex(float isovalue, float3 p0, float3 p1, float s0, float s1) {
	float alpha = (isovalue - s0) / (s1 - s0);
	return alpha * p1 + (1 - alpha) * p0;
}

float3 calculate_normal(global const float* volume, size_t resolution, float grid_length, float3 p, size_t sample_resolution, float step_size) {
	float3 gradient = (float3)(
		(sample(volume, resolution, grid_length, p + (float3)(step_size, 0.f, 0.f), sample_resolution) - sample(volume, resolution, grid_length, p - (float3)(step_size, 0.f, 0.f), sample_resolution)) / (2.f * step_size),
		(sample(volume, resolution, grid_length, p + (float3)(0.f, step_size, 0.f), sample_resolution) - sample(volume, resolution, grid_length, p - (float3)(0.f, step_size, 0.f), sample_resolution)) / (2.f * step_size),
		(sample(volume, resolution, grid_length, p + (float3)(0.f, 0.f, step_size), sample_resolution) - sample(volume, resolution, grid_length, p - (float3)(0.f, 0.f, step_size), sample_resolution)) / (2.f * step_size)
	);

	return -normalize(gradient);
}

void generate_triangles(global const float* volume,
												size_t resolution,
												float grid_length,
												struct cell grid_cell,
												size_t sample_resolution,
												global struct basic_vertex* vertices,
												global volatile int* vertex_count) {
	int cube_index = 0;
	float isovalue = 0.f;
	if (grid_cell.values[0] < isovalue) { cube_index |= 1; }
	if (grid_cell.values[1] < isovalue) { cube_index |= 2; }
	if (grid_cell.values[2] < isovalue) { cube_index |= 4; }
	if (grid_cell.values[3] < isovalue) { cube_index |= 8; }
	if (grid_cell.values[4] < isovalue) { cube_index |= 16; }
	if (grid_cell.values[5] < isovalue) { cube_index |= 32; }
	if (grid_cell.values[6] < isovalue) { cube_index |= 64; }
	if (grid_cell.values[7] < isovalue) { cube_index |= 128; }

	if (cube_index == 0) {
		return;
	}

	float sample_rate = grid_length / sample_resolution;
	float half_sample_rate = sample_rate / 2;

	struct basic_vertex vertex_list[12];
	if (edge_table[cube_index] & 1) {
		float3 vertex = interpolate_vertex(isovalue, grid_cell.vertices[0], grid_cell.vertices[1], grid_cell.values[0], grid_cell.values[1]);
		vertex_list[0].position = vertex;
		vertex_list[0].normal = calculate_normal(volume, resolution, grid_length, vertex, sample_resolution, half_sample_rate);
	}
	if (edge_table[cube_index] & 2) {
		float3 vertex = interpolate_vertex(isovalue, grid_cell.vertices[1], grid_cell.vertices[2], grid_cell.values[1], grid_cell.values[2]);
		vertex_list[1].position = vertex;
		vertex_list[1].normal = calculate_normal(volume, resolution, grid_length, vertex, sample_resolution, half_sample_rate);
	}
	if (edge_table[cube_index] & 4) {
		float3 vertex = interpolate_vertex(isovalue, grid_cell.vertices[3], grid_cell.vertices[2], grid_cell.values[3], grid_cell.values[2]);
		vertex_list[2].position = vertex;
		vertex_list[2].normal = calculate_normal(volume, resolution, grid_length, vertex, sample_resolution, half_sample_rate);
	}
	if (edge_table[cube_index] & 8) {
		float3 vertex = interpolate_vertex(isovalue, grid_cell.vertices[3], grid_cell.vertices[0], grid_cell.values[3], grid_cell.values[0]);
		vertex_list[3].position = vertex;
		vertex_list[3].normal = calculate_normal(volume, resolution, grid_length, vertex, sample_resolution, half_sample_rate);
	}
	if (edge_table[cube_index] & 16) {
		float3 vertex = interpolate_vertex(isovalue, grid_cell.vertices[4], grid_cell.vertices[5], grid_cell.values[4], grid_cell.values[5]);
		vertex_list[4].position = vertex;
		vertex_list[4].normal = calculate_normal(volume, resolution, grid_length, vertex, sample_resolution, half_sample_rate);
	}
	if (edge_table[cube_index] & 32) {
		float3 vertex = interpolate_vertex(isovalue, grid_cell.vertices[5], grid_cell.vertices[6], grid_cell.values[5], grid_cell.values[6]);
		vertex_list[5].position = vertex;
		vertex_list[5].normal = calculate_normal(volume, resolution, grid_length, vertex, sample_resolution, half_sample_rate);
	}
	if (edge_table[cube_index] & 64) {
		float3 vertex = interpolate_vertex(isovalue, grid_cell.vertices[6], grid_cell.vertices[7], grid_cell.values[6], grid_cell.values[7]);
		vertex_list[6].position = vertex;
		vertex_list[6].normal = calculate_normal(volume, resolution, grid_length, vertex, sample_resolution, half_sample_rate);
	}
	if (edge_table[cube_index] & 128) {
		float3 vertex = interpolate_vertex(isovalue, grid_cell.vertices[7], grid_cell.vertices[4], grid_cell.values[7], grid_cell.values[4]);
		vertex_list[7].position = vertex;
		vertex_list[7].normal = calculate_normal(volume, resolution, grid_length, vertex, sample_resolution, half_sample_rate);
	}
	if (edge_table[cube_index] & 256) {
		float3 vertex = interpolate_vertex(isovalue, grid_cell.vertices[0], grid_cell.vertices[4], grid_cell.values[0], grid_cell.values[4]);
		vertex_list[8].position = vertex;
		vertex_list[8].normal = calculate_normal(volume, resolution, grid_length, vertex, sample_resolution, half_sample_rate);
	}
	if (edge_table[cube_index] & 512) {
		float3 vertex = interpolate_vertex(isovalue, grid_cell.vertices[1], grid_cell.vertices[5], grid_cell.values[1], grid_cell.values[5]);
		vertex_list[9].position = vertex;
		vertex_list[9].normal = calculate_normal(volume, resolution, grid_length, vertex, sample_resolution, half_sample_rate);
	}
	if (edge_table[cube_index] & 1024) {
		float3 vertex = interpolate_vertex(isovalue, grid_cell.vertices[2], grid_cell.vertices[6], grid_cell.values[2], grid_cell.values[6]);
		vertex_list[10].position = vertex;
		vertex_list[10].normal = calculate_normal(volume, resolution, grid_length, vertex, sample_resolution, half_sample_rate);
	}
	if (edge_table[cube_index] & 2048) {
		float3 vertex = interpolate_vertex(isovalue, grid_cell.vertices[3], grid_cell.vertices[7], grid_cell.values[3], grid_cell.values[7]);
		vertex_list[11].position = vertex;
		vertex_list[11].normal = calculate_normal(volume, resolution, grid_length, vertex, sample_resolution, half_sample_rate);
	}

	for (int i = 0; triangle_table[cube_index][i] != -1; i += 3) {
		int index = atomic_add(vertex_count, 3);
		vertices[index] = vertex_list[triangle_table[cube_index][i]];
		vertices[index + 1] = vertex_list[triangle_table[cube_index][i + 1]];
		vertices[index + 2] = vertex_list[triangle_table[cube_index][i + 2]];
	}
}

kernel void marching_cubes(global const float* volume,
													 uint resolution,
													 uint sample_resolution,
													 float grid_length,
													 global struct basic_vertex* vertices,
													 global volatile int* vertex_count) {
	size_t x = get_global_id(0);
	size_t y = get_global_id(1);
	size_t z = get_global_id(2);
	float sample_rate = grid_length / sample_resolution;
	float3 p = (float3)(x, y , z) * sample_rate;
	struct cell c;
	c.vertices[0] = p;
	c.vertices[1] = p + (float3)(sample_rate, 0.f, 0.f);
	c.vertices[2] = p + (float3)(sample_rate, 0.f, sample_rate);
	c.vertices[3] = p + (float3)(0.f, 0.f, sample_rate);
	c.vertices[4] = p + (float3)(0.f, sample_rate, 0.f);
	c.vertices[5] = p + (float3)(sample_rate, sample_rate, 0.f);
	c.vertices[6] = p + (float3)(sample_rate, sample_rate, sample_rate);
	c.vertices[7] = p + (float3)(0.f, sample_rate, sample_rate);

	c.values[0] = sample(volume, resolution, grid_length, c.vertices[0], sample_resolution);
	c.values[1] = sample(volume, resolution, grid_length, c.vertices[1], sample_resolution);
	c.values[2] = sample(volume, resolution, grid_length, c.vertices[2], sample_resolution);
	c.values[3] = sample(volume, resolution, grid_length, c.vertices[3], sample_resolution);
	c.values[4] = sample(volume, resolution, grid_length, c.vertices[4], sample_resolution);
	c.values[5] = sample(volume, resolution, grid_length, c.vertices[5], sample_resolution);
	c.values[6] = sample(volume, resolution, grid_length, c.vertices[6], sample_resolution);
	c.values[7] = sample(volume, resolution, grid_length, c.vertices[7], sample_resolution);

	generate_triangles(volume, resolution, grid_length, c, sample_resolution, vertices, vertex_count);
}
