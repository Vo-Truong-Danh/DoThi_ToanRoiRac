#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include <vector>

#define MAX 100
#define VOCUC INT_MAX
#define V 5

using namespace std;
struct DOTHI
{
	int soDinh; // SO DINH CUA DO THI
	int trongSo[MAX][MAX]; // TRONG SO CUA MA TRAN DO THI
	char tenDinh[MAX]; // TEN CUA DINH
};

struct Dijkstra
{
	int d[MAX];
	bool daDuyet[MAX];
	int dinhTruoc[MAX];
};

//===================       MENU     ============================
void showMenu()
{
	printf("\n**********************************    MENU    **********************************");
	printf("\n*  1. Xoa dinh va tim duong di ngan nhat (neu co) - Dijkstra                   *");
	printf("\n*  2. Xuat cac thanh phan lien thong                                           *"); 
	printf("\n*  3. Tim Euler                                                                *");
	printf("\n*  4. Thuat toan Prim                                                          *");
	printf("\n*  5. Thuat toan BFS                                                           *");
	printf("\n*  6. Thuat toan brute force tim duong di ngan nha cua chu trinh hamilton     *");           
	printf("\n*  0. Thoat                                                                    *");
	printf("\n********************************************************************************");
}


//============== DOC DU LIEU DO THI TU FILE =====================
void docDuLieuTuFile(DOTHI& g, char* fileName)
{
	FILE* f = fopen(fileName, "rt");
	if (f == NULL) {
		printf("Khong mo duoc file!");
		return;
	}

	fscanf(f, "%d\n", &g.soDinh);
	for (int i = 0; i < g.soDinh; i++)
	{
		fscanf(f, "%c ", &g.tenDinh[i]);
	}

	// doc trong so cua ma tran
	for (int i = 0; i < g.soDinh; i++)
	{
		for (int j = 0; j < g.soDinh; j++)
		{
			fscanf(f, "%d", &g.trongSo[i][j]);
		}
	}
	fclose(f);
}

//============== IN MA TRAN DO THI RA MAN HINH ==================
void printMatrix(DOTHI g)
{
	printf("\t");
	for (int i = 0; i < g.soDinh; i++)
	{
		printf("%3c", g.tenDinh[i]);
	}
	printf("\n\n");

	for (int i = 0; i < g.soDinh; i++)
	{
		printf(" %c\t", g.tenDinh[i]);
		for (int j = 0; j < g.soDinh; j++)
		{
			printf("%3d", g.trongSo[i][j]);
		}
		printf("\n");
	}
}

// TRẢ VỀ INDEX CỦA ĐỈNH ĐƯỢC TRUYỀN VÀO
int getIndexDinh(DOTHI g, char tenDinh)
{
	int indexDinh = -1;
	for (int i = 0; i < g.soDinh; i++)
	{
		if (g.tenDinh[i] == tenDinh)
		{
			indexDinh = i;
			break;
		}
	}
	return indexDinh;
}

//==================  VÕ PHONG  ==========================

//================== CHỨC NĂNG 1 =========================
void xoa1DongMT(DOTHI& g, int k)
{
	for (int i = k; i < g.soDinh; i++)
	{
		for (int j = 0; j < g.soDinh; j++)
		{
			g.trongSo[i][j] = g.trongSo[i+1][j];
		}
	}
}

void xoa1CotMT(DOTHI& g, int k)
{
	for (int i = 0; i < g.soDinh; i++)
	{
		for (int j = k; j < g.soDinh; j++)
		{
			g.trongSo[i][j] = g.trongSo[i][j + 1];
		}
	}
}

void updateTenDinh(DOTHI& g, int k)
{
	for (int i = k; i < g.soDinh; i++)
	{
		g.tenDinh[i] = g.tenDinh[i + 1];
	}
}

//xóa đỉnh được truyền vào
void xoaDinh(DOTHI& g, char dinh)
{
	int k = getIndexDinh(g, dinh);
	if (k == -1)
	{
		printf("\nDinh khong hop le");
		return;
	}
	xoa1DongMT(g, k);
	xoa1CotMT(g, k);
	updateTenDinh(g, k);
	g.soDinh--;
}

void initDijkstra(DOTHI g, Dijkstra& ds)
{
	for (int i = 0; i < g.soDinh; i++)
	{
		ds.d[i] = VOCUC;
		ds.daDuyet[i] = false;
		ds.dinhTruoc[i] = -1;
	}
}

// Kiem tra dinh chua duyet va co khoang cach S - u min
int ktDinhDijkstra(DOTHI g, Dijkstra& dstra)
{
	int min = VOCUC, u = -1;
	for (int i = 0; i < g.soDinh; i++)
	{
		if (dstra.d[i] < min && !dstra.daDuyet[i])
		{
			min = dstra.d[i];
			u = i;
			break;
		}
	}
	return u;
}

void timDuongDi_Dijkstra(DOTHI g, Dijkstra& dstra, char start)
{
	initDijkstra(g, dstra);
	int s = getIndexDinh(g, start);
	if (s == -1)
	{
		return;
	}
	dstra.d[s] = 0;
	for (int i = 0; i < g.soDinh; i++)
	{
		int u = ktDinhDijkstra(g, dstra);
		dstra.daDuyet[u] = true;
		if (u == -1)
		{
			break;
		}
		for (int v = 0; v < g.soDinh; v++)
		{
			if (g.trongSo[u][v] > 0 && dstra.d[v] > (g.trongSo[u][v] + dstra.d[u]))
			{
				dstra.d[v] = g.trongSo[u][v] + dstra.d[u];
				dstra.dinhTruoc[v] = u;
			}
		}
	}

}

void initArrayFalse(DOTHI g, bool duyet[])
{
	for (int i = 0; i < g.soDinh; i++)
	{
		duyet[i] = false;
	}
}

// kiem tra dinh start den end co ton tai duong di khong
bool ktDuongDi_DFS(DOTHI g, bool duyet[], char start, char end)
{
	int s = getIndexDinh(g, start);
	int e = getIndexDinh(g, end);
	duyet[s] = true;
	for (int i = 0; i < g.soDinh; i++)
	{
		if (g.trongSo[s][i] > 0 && !duyet[i])
		{
			if (g.tenDinh[i] == end)
				return true;
			if (ktDuongDi_DFS(g, duyet, g.tenDinh[i], end))  // Điều kiện dừng và xử lý kết quả đệ quy
				return true;
		}
	}
	return false;
}

void printDuongDiNganNhat_Dijkstra(DOTHI g, Dijkstra dstra, char start, char end)
{
	int s = getIndexDinh(g, start);
	int e = getIndexDinh(g, end);
	int index = dstra.dinhTruoc[e];
	char k = g.tenDinh[index];;
	if (s == -1 || e == -1)
	{
		printf("\nDinh khong hop le - printDijkstra");
		return;
	}
	printf("\n(%c - %c) = %d", start, end,dstra.d[e]);
	printf("\n");
	printf("%c", g.tenDinh[e]);
	while (k != start)
	{
		printf(" <---------- %c", k);
		index = dstra.dinhTruoc[index];
		k = g.tenDinh[index];
	}
	printf(" <---------- %c", g.tenDinh[s]);
}

//================== CHỨC NĂNG 2 =========================
void duyetDoThi_DFS(DOTHI g, int dinh, int tpLT, int nhan[])
{
	for (int j = 0; j < g.soDinh; j++)
	{
		if (nhan[j] == 0 && g.trongSo[dinh][j] > 0)
		{
			nhan[j] = nhan[dinh];
			duyetDoThi_DFS(g, j, tpLT, nhan);
		}
	}
}


// Xet lien thong do thi vo huong
void xetLienThong(DOTHI g, int nhan[MAX], int& soTPLT)
{
	for (int i = 0; i < g.soDinh; i++)
	{
		nhan[i] = 0;
	}

	for (int i = 0; i < g.soDinh; i++)
	{
		if (nhan[i] == 0)
		{
			soTPLT++;
			nhan[i] = soTPLT;
			duyetDoThi_DFS(g, i, soTPLT, nhan);
		}
	}
}

void printThanhPhanLienThong(DOTHI g, int nhanLT[], int soTPLT)
{
	for (int i = 1; i <= soTPLT; i++)
	{
		printf("Thanh phan lien thong %d: ", i);
		for (int j = 0; j < g.soDinh; j++)
		{
			if (nhanLT[j] == i)
			{
				printf("%3c", g.tenDinh[j]);
			}
		}
		printf("\n");
	}
}

//==================          DANH       =========================

//===================       MENU Euler   ============================
void showMenuEuler()
{
	printf("\n***************** Euler's Menu *****************");
	printf("\n\n1. Kiem tra Do Thi VO HUONG hay CO HUONG ");
	printf("\n2. Kiem tra so thanh phan lien thong ");
	printf("\n3. Dem so bac cua dinh X ");
	printf("\n4. Dem so bac cua tat ca cac dinh  ");
	printf("\n5. Kiem tra Euler  ");
	printf("\n0. Tro ve Menu chinh");
	printf("\n\n************************************************");
}

//============== DOC DU LIEU DO THI TU FILE =====================
int docDuLieuTuFileD(DOTHI& g, char* fileName)
{
	FILE* f = fopen(fileName, "rt");
	if (f == NULL) {
		printf("Khong mo duoc file!  ");
		return -1;//Doc file khong thanh cong
	}

	fscanf(f, "%d\n", &g.soDinh);
	for (int i = 0; i < g.soDinh; i++)
	{
		fscanf(f, "%c ", &g.tenDinh[i]);
	}

	// doc trong so cua ma tran
	for (int i = 0; i < g.soDinh; i++)
	{
		for (int j = 0; j < g.soDinh; j++)
		{
			fscanf(f, "%d", &g.trongSo[i][j]);
		}
	}
	fclose(f);
	return 0;//Doc file thanh cong
}

int CheckVoHuong(DOTHI g)// Trả về 1 khi VÔ HƯỚNG và 0 khi CÓ HƯỚNG
{
	for (int i = 0; i < g.soDinh - 1; i++)
		for (int j = i + 1; j < g.soDinh; j++)
			if (g.trongSo[i][j] != g.trongSo[j][i])
				return 0;// Đồ thị Có Hướng 
	return 1;// Đồ thị Vô Hướng
}

void NhapTenFile(DOTHI g, char* fileName)
{
	do
	{
		if (docDuLieuTuFileD(g, fileName) != 0)
			cout << "\n\nTen File khong dung luu long nhap lai !\n";
		cout << "\nNhap ten File do thi : ";
		cin >> fileName;
	} while (docDuLieuTuFileD(g, fileName) != 0);
}

void printfCheckVoHuong(DOTHI g, char* fileName)
{
	NhapTenFile(g, fileName);
	docDuLieuTuFile(g, fileName);
	cout << "\n\n";
	printMatrix(g);
	int tmp = CheckVoHuong(g);
	if (tmp == 1)
		cout << "\n\n\tDo Thi VO HUONG ";
	if (tmp == 0)
		cout << "\n\n\tDo Thi CO HUONG ";
	cout << "\n\n\n";
}

void SearchDLT(int X, DOTHI &g, int LT[])
{
	for (int i = 0; i < g.soDinh; i++)
	{
		if (g.trongSo[i][X] && LT[i] == 0)
		{
			LT[i] = LT[X];
			SearchDLT(i, g, LT);
		}
	}
}

int SearchSoTPLT(DOTHI g)
{
	int LT[MAX] = { 0 };
	int SoTPLT = 0;
	for (int i = 0; i < g.soDinh; i++)
	{
		if (LT[i] == 0)
		{
			SoTPLT++;
			LT[i] = SoTPLT;
			SearchDLT(i, g, LT);
		}
	}
	return SoTPLT;
}

void printfSoTPLT(DOTHI g, char* fileName)
{
	NhapTenFile(g, fileName);
	docDuLieuTuFile(g, fileName);
	cout << "\n\n";
	printMatrix(g);
	if (SearchSoTPLT(g) > 0 && SearchSoTPLT(g) < 998)
	{
		cout << "\n\nSo thanh phan lien thong cua Do Thi la:" << SearchSoTPLT(g);
		cout << "\n\n\n";
	}
}

int SearchSoBacDinh(DOTHI g, int DinhX)
{
	int sumBac = 0;
	for (int i = 0; i < g.soDinh; i++)
	{
		if (g.trongSo[DinhX][i] != 0)
			sumBac++;
	}
	return sumBac;
}

int ChuyenChuThanhSo(DOTHI g, char DinhXC)
{
	for (int i = 0; i < g.soDinh; i++)
	{
		if (g.tenDinh[i] == DinhXC)
			return i;
	}
	return -1;
}

char ChuyenSoThanhChu(DOTHI g, int DinhX)
{
	if (DinhX >= 0 && DinhX < g.soDinh)
	{
		return g.tenDinh[DinhX];
	}
	cout << "\nPhan tu khong ton tai.";
	return ' ';
}

void printfSoBacDinhX(DOTHI g, char* fileName)
{
	NhapTenFile(g, fileName);
	char DinhXC;
	int DinhX;
	docDuLieuTuFile(g, fileName);
	cout << "\n\n";
	printMatrix(g);
	do
	{
		cout << "\n\nNhap ten dinh can tim: ";
		cin >> DinhXC;
		{
			if (ChuyenChuThanhSo(g, DinhXC) == -1)
				cout << "\nTen Dinh khong dung vui long nhap lai !";
		}
	} while (ChuyenChuThanhSo(g, DinhXC) == -1);
	DinhX = ChuyenChuThanhSo(g, DinhXC);
	cout << "\n\nSo Bac cua dinh " << DinhXC << " la :" << SearchSoBacDinh(g, DinhX);
}

void printfSoBacCacDinh(DOTHI g, char* fileName)
{
	NhapTenFile(g, fileName);
	docDuLieuTuFile(g, fileName);
	cout << "\n\n";
	printMatrix(g);
	cout << "\n\t   SO BAC CUA DINH :";
	for (int i = 0; i < g.soDinh; i++)
		cout << "\n\tSo Bac cua dinh " << g.tenDinh[i] << " la :" << SearchSoBacDinh(g, i);
}

int CheckEuler(DOTHI g) {
	int VoHuong = CheckVoHuong(g);
	int SoTPLT = SearchSoTPLT(g);
	int SoBacLe = 0;
	int Euler = 2;  // Mặc định không phải Euler

	// Kiểm tra đồ thị có hướng hay vô hướng
	if (VoHuong == 0) {
		cout << "\n\nDo thi co huong. Khong phai Euler.";
		return Euler;
	}

	// Kiểm tra số thành phần liên thông
	if (SoTPLT != 1) {
		cout << "\n\nDo thi khong lien thong. Khong phai Euler.";
		return Euler;
	}

	// Kiểm tra số bậc lẻ của các đỉnh
	for (int i = 0; i < g.soDinh; i++) {
		if (SearchSoBacDinh(g, i) % 2 == 1) {
			SoBacLe++;
		}
	}

	// Kiểm tra số bậc lẻ
	if (SoBacLe == 0) {
		cout << "\n\nDo thi co chu trinh Euler.";
		Euler = 0;  // Chu trình Euler
	}
	else if (SoBacLe == 2) {
		cout << "\n\nDo thi co duong di Euler.";
		Euler = 1;  // Đường đi Euler
	}
	else {
		cout << "\n\nDo thi khong phai Euler.";
	}

	return Euler;
}

void SearchDD(DOTHI& g, int X, int LKDD[], int& sl)
{
	for (int i = 0; i < g.soDinh; i++)
		if (g.trongSo[X][i] != 0)
		{
			g.trongSo[X][i] = g.trongSo[i][X] = 0;
			SearchDD(g, i, LKDD, sl);
		}
	LKDD[sl++] = X;
}

void printfLKDDEuler(DOTHI g, char* fileName) {
	NhapTenFile(g, fileName);
	docDuLieuTuFile(g, fileName);
	int Euler = CheckEuler(g);
	cout << "\n\n";
	printMatrix(g);

	if (Euler == 2) {
		cout << "\n\nDo thi khong phai Euler.";
		return;
	}

	char DinhXC;
	int DinhBD;

	if (Euler == 0 || Euler == 1) {
		// Sử dụng chu trình Euler hoặc đường đi Euler tùy thuộc vào giá trị của Euler
		if (Euler == 1) {
			// Chọn một đỉnh có bậc lẻ làm đỉnh bắt đầu cho đường đi Euler
			for (int i = 0; i < g.soDinh; i++) {
				if (SearchSoBacDinh(g, i) % 2 == 1) {
					DinhBD = i;
					break;
				}
			}
		}
		else {
			cout << "\n\nChon dinh Bat Dau cua " << (Euler == 0 ? "Chu Trinh Euler" : "Duong di Euler") << ": ";
			cin >> DinhXC;
			DinhBD = ChuyenChuThanhSo(g, DinhXC);
		}
	}

	int LKDD[MAX];
	int sl = 0;
	SearchDD(g, DinhBD, LKDD, sl);

	if (Euler == 0 || Euler == 1) {
		cout << "\n\t " << (Euler == 0 ? "Chu Trinh Euler" : "Duong di Euler") << " la : ";
	}

	for (int i = sl - 1; i >= 0; i--) {
		cout << ChuyenSoThanhChu(g, LKDD[i]);

		if (i > 0) {
			cout << " -> ";
		}
	}

	cout << endl;
}

//==================          MINH       =========================

bool is_valid(int v, int pos, const vector<vector<int>>& graph, vector<int>& path) {
	// Kiểm tra xem có thể kết nối đỉnh v với đỉnh đang xét không
	if (!graph[path[pos - 1]][v]) {
		return false;
	}

	// Kiểm tra xem đỉnh v đã được thăm chưa
	if (find(path.begin(), path.end(), v) != path.end()) {
		return false;
	}

	return true;
}

void printHamiltonianCycle(const vector<int>& path) {
	cout << "Chu trinh Hamilton ngan nhat: ";
	for (int i = 0; i < path.size(); ++i) {
		cout << path[i] << " ";
	}
	cout << path[0] << endl;
}

bool hamiltonian_util(const vector<vector<int>>& graph, vector<int>& path, int pos) {
	int num_vertices = graph.size();

	if (pos == num_vertices) {
		// Nếu đã đi qua tất cả các đỉnh, kiểm tra xem có tạo thành chu trình Hamilton không
		if (graph[path[pos - 1]][path[0]]) {
			return true;
		}
		return false;
	}

	for (int v = 1; v < num_vertices; ++v) {
		if (is_valid(v, pos, graph, path)) {
			path[pos] = v;

			if (hamiltonian_util(graph, path, pos + 1)) {
				return true;
			}

			// Backtrack nếu không tìm thấy chu trình Hamilton với đỉnh hiện tại
			path[pos] = -1;
		}
	}

	return false;
}

void hamiltonian(const vector<vector<int>>& graph) {
	int num_vertices = graph.size();

	// Khởi tạo đường đi Hamilton
	vector<int> path(num_vertices, -1);

	// Bắt đầu từ đỉnh 0
	path[0] = 0;

	if (!hamiltonian_util(graph, path, 1)) {
		cout << "Khong ton tai chu trinh Hamilton." << endl;
	}
	else {
		printHamiltonianCycle(path);
	}
}

//================================================================


//==================          HOÀ       =========================

int minKey(int key[], bool mstSet[])
{

	int min = INT_MAX, min_index;

	for (int v = 0; v < V; v++)
		if (mstSet[v] == false && key[v] < min)
			min = key[v], min_index = v;

	return min_index;
}


void printMST(int parent[], int graph[V][V])
{
	printf("Edge \tWeight\n");
	for (int i = 1; i < V; i++)
		printf("%d - %d \t%d \n", parent[i], i,
			graph[i][parent[i]]);
		
}


void primMST(int graph[V][V])
{

	int parent[V];

	int key[V];

	bool mstSet[V];


	for (int i = 0; i < V; i++)
		key[i] = INT_MAX, mstSet[i] = false;


	key[0] = 0;


	parent[0] = -1;


	for (int count = 0; count < V - 1; count++) {


		int u = minKey(key, mstSet);


		mstSet[u] = true;


		for (int v = 0; v < V; v++)


			if (graph[u][v] && mstSet[v] == false
				&& graph[u][v] < key[v])
				parent[v] = u, key[v] = graph[u][v];
	}


	printMST(parent, graph);
}


//==================       ANH PHONG      ========================
// Cau truc dinh
struct DINH {
	char ten[MAX];
};


struct DOTHI_P {
	int soDinh; // So dinh cua do thi
	int trongSo[MAX][MAX]; // Trong so cua ma tran do thi
	struct DINH danhSachDinh[MAX]; // Danh sách dinh cua do thi
};

//
struct HangDoi {
	int front, rear;
	int dinh[MAX];
};

// Khoi tao hang doi
void KhoiTaoHangDoi(struct HangDoi* q) {
	q->front = q->rear = -1;
}

// Kiem tra hang doi co trong hay ko
bool HangDoiRong(struct HangDoi* q) {
	return q->front == -1;
}

// Thêm mot dinh vao hang doi
void ThemVaoHangDoi(struct HangDoi* q, int dinh) {
	if (q->rear == MAX - 1) {
		printf("Hang doi day\n");
		return;
	}
	if (q->front == -1) {
		q->front = 0;
	}
	q->rear++;
	q->dinh[q->rear] = dinh;
}

// Lay 1 dinh tu hang doi
int LayTuHangDoi(struct HangDoi* q) {
	if (q->front == -1) {
		printf("Hang doi rong\n");
		return -1;
	}
	int dinh = q->dinh[q->front];
	q->front++;
	if (q->front > q->rear) {
		q->front = q->rear = -1;
	}
	return dinh;
}

// Kiem tra 1 dinh co them vao hang doi hay ko
bool CoTheThemVaoHangDoi(struct HangDoi* q, int dinh) {
	for (int i = q->front; i <= q->rear; i++) {
		if (q->dinh[i] == dinh) {
			return false;
		}
	}
	return true;
}

void BFS(struct DOTHI_P* g, int startDinh) {
	struct HangDoi q;
	KhoiTaoHangDoi(&q);

	bool daTham[MAX];
	memset(daTham, false, sizeof(daTham));

	ThemVaoHangDoi(&q, startDinh);
	daTham[startDinh] = true;

	printf("Ket qua cua BFS: ");

	while (!HangDoiRong(&q)) {	
		int dinhHienTai = LayTuHangDoi(&q);
		printf("%s ", g->danhSachDinh[dinhHienTai].ten);

		for (int i = 0; i < g->soDinh; i++) {
			if (g->trongSo[dinhHienTai][i] != 0 && !daTham[i]) {
				ThemVaoHangDoi(&q, i);
				daTham[i] = true;
			}
		}
	}

	printf("\n");
}
//================================================================

void main()
{
	DOTHI g;
	char fileName[MAX] = "data.txt";
	int luaChon;
	do
	{
		system("cls"); // xóa màn hình console
		showMenu();
		printf("\nNhap vao lua chon: ");
		scanf("%d", &luaChon);
		switch (luaChon)
		{
		case 1:
		{
			system("cls");
			Dijkstra dstra;
			char deleteDinh, start, end;
			int nDelete;
			bool duyet[MAX];

			docDuLieuTuFile(g, fileName);
			printf("\nMa tran goc:\n\n");
			printMatrix(g);
			printf("\n");
			printf("So luong dinh can xoa: ");
			scanf("%d", &nDelete);
			for (int i = 0; i < nDelete; i++)
			{
				printf("Dinh %d:",i+1);
				getchar();
				scanf("%c", &deleteDinh);
				xoaDinh(g, deleteDinh);
			}
			printf("\n\n");
			printMatrix(g);

			printf("\nNhap dinh bat dau: ");
			getchar();
			scanf("%c", &start);
			printf("Nhap dinh ket thuc: ");
			getchar();
			scanf("%c", &end);

			initArrayFalse(g, duyet);
			if (ktDuongDi_DFS(g, duyet, start, end) == true)
			{
				timDuongDi_Dijkstra(g, dstra, start);
				printDuongDiNganNhat_Dijkstra(g, dstra, start, end);
			}
			else
				printf("\nKhong co duong di de tim");
			_getch();

			break;
		}
		case 2:
		{
			system("cls");
			int nhanLT[MAX], soTPLT = 0;
			docDuLieuTuFile(g, fileName);
			printMatrix(g);
			printf("\n\n");

			xetLienThong(g, nhanLT, soTPLT);
			printf("Thanh phan lien thong: %d\n", soTPLT);
			printThanhPhanLienThong(g, nhanLT, soTPLT);
			_getch();
			break;
		}
		case 3:
		{
			int luachonEuler;
			do
			{
				system("cls"); // xóa màn hình console
				showMenuEuler();
				printf("\nNhap vao lua chon: ");
				scanf("%d", &luachonEuler);

				switch (luachonEuler)
				{
				case 1:
					printfCheckVoHuong(g, fileName);
					_getch();
					break;
				case 2:
					printfSoTPLT(g, fileName);
					_getch();
					break;
				case 3:
					printfSoBacDinhX(g, fileName);
					_getch();
					break;
				case 4:
					printfSoBacCacDinh(g, fileName);
					_getch();
					break;
				case 5:
					printfLKDDEuler(g, fileName);
					_getch();
					break;
				case 0:
					printf("\nBan se tro ve Menu chinh !");
					_getch();
					break;
				default:
					printf("\nVui long nhap lai");
					_getch();
					break;
				}
			} while (luachonEuler != 0);
			break;
		}
		case 4:
		{
			int graph[V][V] = { { 0, 2, 0, 6, 0 },
					   { 2, 0, 3, 8, 5 },
					   { 0, 3, 0, 0, 7 },
					   { 6, 8, 0, 0, 9 },
					   { 0, 5, 7, 9, 0 } };
			primMST(graph);
			_getch();
			break;
		}
		case 5:
		{
			struct DOTHI_P g;

			// Nhap so dinh cua do thi
			printf("Nhap so dinh cua do thi: ");
			scanf("%d", &g.soDinh);

			// Nhap trong so cua ma tran do thi
			printf("Nhap ma tran trong so cua do thi:\n");
			for (int i = 0; i < g.soDinh; i++) {
				for (int j = 0; j < g.soDinh; j++) {
					printf("Trong so cua dinh %d den dinh %d: ", i + 1, j + 1);
					scanf("%d", &g.trongSo[i][j]);
				}
			}

			// Nhap ten cac dinh
			for (int i = 0; i < g.soDinh; i++) {
				printf("Nhap ten cua dinh %d: ", i + 1);
				scanf("%s", g.danhSachDinh[i].ten);
			}

			// Chon dinh xuat phat
			int startDinh;
			printf("Nhap dinh bat dau cho BFS: ");
			scanf("%d", &startDinh);

			// Duyet do thi bang BFS
			BFS(&g, startDinh - 1);
			_getch();
			break;
		}
		case 6:
		{
			// Đồ thị được biểu diễn bằng ma trận kề
			cout << "Vi du voi do thi nhu sau:\n";
			cout << "0  1  1\n3  1  2\n3  2  0\n";
			vector<vector<int>> sample_graph = {
				{0, 1, 1},
				{3, 1, 2},
				{3, 2, 0}
			};

			hamiltonian(sample_graph);
			_getch();
			break;
		}
		case 0:
			printf("\nOut success!");
			_getch();
			return;
		default:
			printf("\nVui long nhap lai");
			_getch();
			break;
		}
	} while (1);

}