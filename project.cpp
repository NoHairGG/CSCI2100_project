#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

using namespace std;

class bst {
private:
	int value;
	bst* leftChild;
	bst* rightChild;
public:
	bst();
	bst(int n); // set value;
	void insert(int n);
	void build(int* arr, int st, int ed);
	void inOrder(bst* root);
	void search(ostream& out, bst* root, bst* pred, int target);
};

//default constructor
bst::bst() {
	value = 0;
	leftChild = NULL;
	rightChild = NULL;
}

bst::bst(int n) {
	value = n;
	leftChild = NULL;
	rightChild = NULL;
}

void bst::insert(int n) {
	if (this->value == 0) {
		this->value = n;
	}
	else {
		//n > this->value
		if (n > this->value) {
			if (this->rightChild == NULL) {
				bst* newLeaf = new bst(n);
				this->rightChild = newLeaf;
			}
			else {
				this->rightChild->insert(n);
			}
		}
		//n < this->value
		if (n < this->value) {
			if (this->leftChild == NULL) {
				bst* newLeaf = new bst(n);
				this->leftChild = newLeaf;
			}
			else {
				this->leftChild->insert(n);
			}
		}
	}
}

//special set up for special balanced bst
void bst::build(int* arr, int st, int ed) {
	if (st <= ed) {
		int mid = (st + ed) / 2;
		//cout << st << " " << ed << " " << arr[mid] << endl;
		insert(arr[mid]);
		build(arr, st, mid - 1);
		build(arr, mid + 1, ed);
	}
}

//for testing my tree
void bst::inOrder(bst* root)
{
	if (!root) {
		return;
	}
	inOrder(root->leftChild);
	cout << root->value << "\n";
	inOrder(root->rightChild);
}

//predecessor
void bst::search(ostream& out, bst* root, bst* prec, int target) {
	if (root != NULL) {
		if (target > root->value) {
			prec = root;
			return search(out, root->rightChild, prec, target);
		}
		else {
			return search(out, root->leftChild, prec, target);
		}
	}
	else {
		if (prec == NULL) {
			out << "no" << "\n";
		}
		else {
			out << prec->value << "\n";
		}
	}
}

//my qsort implementation take reference from the internet
void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int part(int* arr, int st, int ed) {
	int token = arr[ed];
	int i = st - 1;
	for (int j = st; j < ed; j++) {
		if (arr[j] < token) {
			i++;
			swap(&arr[i], &arr[j]);
		}
	}
	i++;
	swap(&arr[i], &arr[ed]);
	return i;
}

void qsort(int* arr, int st, int ed) {
	if (st < ed) {
		int token = part(arr, st, ed);
		qsort(arr, st, token - 1);
		qsort(arr, token + 1, ed);
	}
}

int inArr[1000001];

int main() {
	for (int i = 0; i < 1000001; i++) {
		inArr[i] = 0;
	}
	ifstream fin;
	fin.open("ds.txt", ifstream::in);
	string kekw;
	int n = 0, arrLength = 1, tmp = 0, start = 0, end = 0;
	double duration[5] = { 0,0,0,0,0 };
	start = clock();
	//make use of file input, dynamic array expansion and quick sort to prepare the sorted array
	//build the sorted list
	while (!fin.eof()) {
		fin >> tmp;
		// reference from www.cplusplus.com/forum/general/11111
		// reallocate the dynamic array
		inArr[n] = tmp;
		n = n + 1;
	}
	fin.close();
	qsort(inArr, 0, n - 1);
	//time testing
	end = clock();
	duration[0] = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "File input data.txt and quick sort, spent time: " << duration[0] << " seconds. " << endl;
	//build my tree with the sorted array
	bst* tree = new bst;
	tree->build(inArr, 0, n - 1);
	//tree->inOrder(tree);

	//time testing
	end = clock();
	duration[1] = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "Build BST, spent time: " << duration[1] - duration[0] << " seconds. " << endl;

	//query part
	n = 0, tmp = 0;
	fin.open("qry.txt", ifstream::in);
	while (!fin.eof()) {
		//just for lazy people like me
		fin >> kekw;
		fin >> tmp;
		inArr[n] = (int)tmp;
		n = n + 1;
	}
	fin.clear();
	fin.close();
	//time testing
	end = clock();
	duration[2] = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "File input data, spent time: " << duration[2] - duration[1] << " seconds. " << endl;

	ofstream fout;
	fout.open("output.txt", ofstream::out | ofstream::trunc);
	for (int i = 0; i < n - 1; i++) {
		tree->search(fout, tree, NULL, inArr[i]);
		fout.clear();
	}
	end = clock();
	duration[3] = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "Searching and file output spent time: " << duration[3] - duration[2] << " seconds. " << endl;
	fout.close();
	duration[4] = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "Overall time: " << duration[4] << " seconds. " << endl;
	//system("pause");
	return 0;
}