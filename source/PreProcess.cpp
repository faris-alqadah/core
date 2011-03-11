#include "../Headers/PreProcess.h"


void Tokenize(const string& str, vector<string>& tokens,
                      const string& delimiters){
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)  {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


void ReadInputFile(){
          ifstream myfile(inputFile.c_str());
          if (myfile.is_open()){
              string line;
              getline (myfile,line);
              N = atoi(line.c_str());     //first line is N
              //resize all vectors
              contextVector.resize(N-1);
              contextFiles.resize(N-1);
              nameFiles.resize(N);
              nameMapVector.resize(N);
              numElements.resize(N);
              setNames.resize(N);
              //get set names and thier name files
              for(int i=0; i < N; i++){
                  getline(myfile,line);
                  GetSetNameNumEntries(line,i);
                  getline(myfile,line);
                  nameFiles[i] =line;
                  nameMapVector[i] = new NameMap(nameFiles[i],numElements[i]);
              }
              //get relations and contexts
              for(int i=0; i < (N-1); i++){
                 // cout<<"\nreading context: "<<i;
                  getline(myfile,line);
                  string line1 = line;
                  getline(myfile,line);
                  contextFiles[i] = line;
                  GetRelationGraph(line1,i);
              }
	      myfile.close();
          }
	  else{
	  	cerr<<"\nCould not open the input file: "<<inputFile<<"!!";
                exit(-1);
	  }

}

void MakeContext(int rowsId, int colsId, int ctxId){
    //open context file
    string ctxFile=contextFiles[ctxId];
    ifstream ctxStream(ctxFile.c_str());
    vector<IOSet*> rows;
    vector<IOSet*> cols;
    if(ctxStream.is_open()){
        string line;
        int numRows= numElements[rowsId];
        int numCols= numElements[colsId];
        rows.resize(numRows);
        for(int i=0; i < numRows; i++){
            rows[i] = new IOSet;
            getline(ctxStream,line);
            vector<string> colIdxsStr;
            Tokenize(line,colIdxsStr," ");
            if(colIdxsStr.size() >= 1){        //this row is non-empty
                for(int j=0; j < colIdxsStr.size();j++){
                    if(atoi(colIdxsStr[j].c_str()) > 0)
                        rows[i]->Add( atoi(colIdxsStr[j].c_str()) -1);
                }
            }
        }
        //done getting rows
        //now get columns
        cols.resize(numCols);
        for(int i=0; i < numCols; i++) cols[i] = new IOSet;
        for(int i=0; i < rows.size(); i++){
            for(int j=0; j < rows[i]->Size();j++){
                int currColIdx= rows[i]->At(j);
                if (rows[i]->At(j) >= numCols){
                    cerr<<"\nERROR: Row contains an index ( "<<rows[i]->At(j)
                        <<" ) greater than the number of columns! Check input FIMI file OR input file with number of columns!\n";
                    exit(-1);

                }
                cols[currColIdx]->Add(i);
            }
        }
        contextVector[ctxId] = new Context(rows,cols);
        contextVector[ctxId]->SetRowsId(rowsId);
        contextVector[ctxId]->SetColsId(colsId);
        contextVector[ctxId]->SetId(ctxId);
    }else{
        cerr<<"\nERROR: Could not open context file "<<ctxFile<<" !!!\n"<<endl;
        exit(-1);
    }
}


void GetSetNameNumEntries(const string&line,int num){
    vector<string> currNums;
    Tokenize(line,currNums,";");
    if(currNums.size() == 2){
        setNames[num] = currNums[0];
        numElements[num] = atoi(currNums[1].c_str());
    }
    else{
        cerr<<"\nERROR IN SPECIFYING NUM ELEMENTS IN SET!\n";
        exit(-1);
    }
}

 void GetRelationGraph(const string &line, int ctxId){
    bool error = false;
    vector<string> currSetNames;
    string ctxName;
    Tokenize(line,currSetNames,"--");
    if(currSetNames.size()  == 2){
        int nums[2];
        for(int i=0; i < 2; i++){
            vector<string>::iterator it;
            it = find (setNames.begin(),setNames.end(), currSetNames[i]);
            int pos;
            if (it != setNames.end()) pos = it-setNames.begin();
            else{
                error=true;
                break;
            }
            nums[i] =pos;
        }
        if (!error){
            MakeContext(nums[0],nums[1],ctxId);
            RelationNode *a;
            RelationNode *b;
            if(relationGraph.ContainsNode(nums[0]))
               a = relationGraph.GetNode(nums[0]);
            else{
                a = new RelationNode(nums[0],currSetNames[0]);
                relationGraph.AddNode(a);
            }
            if (relationGraph.ContainsNode(nums[1]))
                b=relationGraph.GetNode(nums[1]);
            else{
                b= new RelationNode(nums[1],currSetNames[1]);
                relationGraph.AddNode(b);
            }
            a->AddEdge(b,contextVector[ctxId]);
            b->AddEdge(a,contextVector[ctxId]);
            ctxName = currSetNames[0];
            ctxName.append("_");
            ctxName.append(currSetNames[1]);
            contextVector[ctxId]->SetName(ctxName);
             
        }
    }else error=true;
    if(error){
        cerr<<"\nERROR IN SPECIFYING RELATION ! \n";
        exit(-1);

    }
}






void OutputSetStats(){
    cout<<"\n\n@@@@@@@@@@@@@@@SETS@@@@@@@@@@@@@@@@\n";
    for(int i=0; i < N; i++){
        cout<<"\nSET "<<i+1<<"\t "<<setNames[i]<<"\t "<<numElements[i]<<" elements";
    }

}

void OutputContextStats(){
    cout<<"\n\n@@@@@@@@@@@@@@@CONTEXTS@@@@@@@@@@@@@@@@\n";
    for(int i=0; i < N-1; i++){
        cout<<"\nCONTEXT "<<contextVector[i]->GetId()<<"\t "<<contextVector[i]->GetName()<<"\t "
            <<numElements[contextVector[i]->GetRowsId()]
            <<" X "<<numElements[contextVector[i]->GetColsId()]
            <<"\t NUM ONES: "<<contextVector[i]->GetNumOnes()<<"\tDensity: "<<contextVector[i]->GetDensity();
    }
}

