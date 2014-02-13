/* read_write.cc	*/

void kleinian_group::read_from_file(ifstream &input_file){		// read data from file

//	input_file >> data;
};


void kleinian_group::write_triangles_to_file(){		// write data to file
	// write triangle and normal data to file
	int i,j;
	ofstream output_file;
	
	output_file.open("output_triangles.tri");
	
	for(i=0;i<(int) DRAW_TRIANGLES.size();i++){
		for(j=0;j<3;j++){
			output_file << DRAW_TRIANGLES[i].v[j][0] << " " << DRAW_TRIANGLES[i].v[j][1] << " "
				<< DRAW_TRIANGLES[i].v[j][2] << "\n";
		};
		output_file << DRAW_NORMALS[i][0] << " " <<  DRAW_NORMALS[i][1] << " "
			 << DRAW_NORMALS[i][2] << "\n";
	};
	output_file.close();	
};