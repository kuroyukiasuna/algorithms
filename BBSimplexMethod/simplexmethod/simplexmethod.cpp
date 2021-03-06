// simplexmethod.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Eigen/Dense>
#include <algorithm>
#include <vector>
#include <math.h>
#include <fstream>
using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;


class dualSimplex {
	int n;
	int m;
	vector<int> resultArray;
	vector<int> Bi;
	vector<int> Li;
	vector<int> Ui;
	VectorXd c; int sizec = 0;
	VectorXd l; int sizel = 0;
	VectorXd u; int sizeu = 0;
	VectorXd b; int sizeb = 0;
	VectorXd cBar;
	VectorXd xb;
	VectorXd resultX;
	MatrixXd A;
	MatrixXd B;
	MatrixXd L;
	MatrixXd U;
	double tol;
	bool infeas = false;
	bool feas = false;

public:
	dualSimplex(){}

	dualSimplex(int m, int n, double tol) :n{ n }, m{ m }, tol{ tol } {
		Bi = vector<int>(m);
		resultArray = vector<int>(n + m);
		c = VectorXd(m + n);
		l = VectorXd(m + n);
		u = VectorXd(m + n);
		b = VectorXd(m);
		A = MatrixXd(m, m + n);
		resultX = VectorXd(m + n);
		B = MatrixXd::Identity(m, m);
	}

	dualSimplex(const dualSimplex &other) { //copy constructor
		resultArray = other.resultArray;
		sizec = other.sizec;
		sizeb = other.sizeb;
		sizel = other.sizel;
		sizeu = other.sizeu;
		Li = other.Li;
		Ui = other.Ui;
		Bi = other.Bi;
		m = other.m;
		n = other.n;
		c = other.c;
		l = other.l;
		u = other.u;
		b = other.b;
		cBar = other.cBar;
		xb = other.xb;
		A = other.A;
		B = other.B;
		L = other.L;
		U = other.U;
		tol = other.tol;
		resultX = other.resultX;
	}

	void changeLowerBound(int j, double num) { //---------------------------------
		l(j) = num;
	}

	void changeUpperBound(int j, double num) {//implemented for the BB algorithm 
		u(j) = num;
	}

	double getUpperBound(int j) {
		return u(j);
	}

	double getLowerBound(int j) {
		return l(j);
	}

	bool isFeasible() {
		return feas;
	}

	bool isInfeasible() {
		return infeas;
	}

	int getNonint() {
		int j = -1;
		for (int i = 0; i < resultX.size(); i++) {
			if (abs(resultX(i) - round(resultX(i))) > tol) {
				j = i;
				break;
			}
		}
		return j;
	}

	VectorXd getResultX() { //-------------------------------------------------
		VectorXd result(n);
		for (int i = 0; i < n; i++) {
			result(i) = resultX(i);
		}
		return result;
	}

	void addToVec(double num, int indicator) { //add num to the vectors
		switch (indicator) {
		case 1: 
			c[sizec] = num;
			sizec++;
			break;
		case 2: 
			l[sizel] = num;
			sizel++;
			break;
		case 3: 
			u[sizeu] = num;
			sizeu++;
			break;
		case 4:
			b[sizeb] = num;
			sizeb++;
			break;
		default: return;
		}
	}

	void addToMat(double num, int row, int col) { //add num to A
		A(row, col) = num;
	}

	void formulateProblem() { //enlarge the problem for dual simplex method
		sizec = m + n;
		sizel = m + n;
		sizeu = m + n;
		for (int i = n; i < n + m; i++) {
			c[i] = 0;
			l[i] = 0;
			u[i] = 0;
			resultArray[i] = 0;
			resultX(i) = 0;
			Bi[i - n] = i;
		}
		A.rightCols(m) = MatrixXd::Identity(m, m);

		formBasisB();
		calculateCBar();

	   for (int j = 0; j < n; j++) {
			if (cBar[j] > 0) {
				Li.push_back(j);
				resultArray[j] = -1;
			}
			else if(cBar[j] < 0) {
				Ui.push_back(j);
				resultArray[j] = 1;
			}
			else {
				Li.push_back(j);
				resultArray[j] = -1;
			}
			resultX(j) = 0;
		}
	}

	void formBasisB() { //update B according to basis Bi
		sort(Bi.begin(), Bi.end());
		for (int i = 0; i < m; i++) {
			B.col(i) = A.col(Bi[i]);
		}
	}

	void formUpperBound() { //update U according to upperbound Ui
		sort(Ui.begin(), Ui.end());
		U = MatrixXd(m, Ui.size());
		for (int i = 0; i < Ui.size(); i++) {
			U.col(i) = A.col(Ui[i]);
		}
	}

	void formLowerBound() { //update L according to lowerbound Li
		sort(Li.begin(), Li.end());
		L = MatrixXd(m, Li.size());
		for (int i = 0; i < Li.size(); i++) {
			L.col(i) = A.col(Li[i]);
		}
	}



	/*MatrixXd formMatrixAccoringToBasis(VectorXd &basis, MatrixXd &original) {
		MatrixXd result(m, basis.size());
		for (int i = 0; i < basis.size(); i++) {
			result.col(i) = original.col(basis(i));
		}
		return result;
	}*/

	VectorXd formVectorAccoringToBasis(vector<int> &basis, VectorXd &original) { //get l_Li or u_Ui
		VectorXd result(basis.size());
		for (int i = 0; i < basis.size(); i++) {
			result[i] = original(basis[i]);
		}
		return result;
	}

	void calculateCBar() { //calculate CBar
		cBar = c.transpose() - (formVectorAccoringToBasis(Bi, c).transpose() * B.inverse() * A);
	}

	void calculateXB() { //calculate xb 
		VectorXd tempLi = formVectorAccoringToBasis(Li, l);
		VectorXd tempUi = formVectorAccoringToBasis(Ui, u);
		xb = B.inverse() * (b - L*tempLi - U*tempUi);
	}

	double calculateObj() {
		double result = 0;
		for (int i = 0; i < Bi.size(); i++) {
			result += c(Bi[i]) * xb(i);
			resultX(Bi[i]) = xb(i);
		}
		for (int i = 0; i < Li.size(); i++) {
			result += c(Li[i]) * l(Li[i]);
			resultX(Li[i]) = l(Li[i]);
		}
		for (int i = 0; i < Ui.size(); i++) {
			result += c(Ui[i]) * u(Ui[i]);
			resultX(Ui[i]) = u(Ui[i]);
		}
		return result;
	}

	/*void TestResult() {
		double result = 0;
		for (int j = 0; j < m; j++) {
			VectorXd temp = A.row(j);
			for (int i = 0; i < Bi.size(); i++) {
				result += temp(Bi[i]) * xb(i);
			}
			for (int i = 0; i < Li.size(); i++) {
				result += temp(Li[i]) * l(Li[i]);
			}
			for (int i = 0; i < Ui.size(); i++) {
				result += temp(Ui[i]) * l(Ui[i]);
			}
			cout << result << endl;
		}
	}

	void printbool(bool in) {
		if (in) {
			cout << "left" << endl;
		}
		else {
			cout << "right" << endl;
		}
	}*/

	/*void printResult(ostream& ff) { //print the result on the outputfile
		for (int i = 0; i < m + n; i++) {
			ff << resultArray[i] << " ";
		}
	}*/

	void dualSimplexMethod() {
		while (true) {
			formBasisB();
			formUpperBound();
			formLowerBound();
			calculateCBar();
			calculateXB();

			int i = -1;
			int j = -1;
			int g = 0;
			bool lFlagBi = true;
			double price = 0;
			while (g < xb.size()) {  //choosing the leaving variable using the pricing rule
				if (xb(g) < l(Bi[g]) - tol) {
					if (abs((l(Bi[g]) - tol) - xb(g)) > price) {
						lFlagBi = true;
						i = g;
						j = Bi[g];
					}
				}
				else if (xb(g) > u(Bi[g]) + tol) {
					if (abs(xb(g) - (u(Bi[g]) + tol)) > price) {
						j = Bi[g];
						i = g;
						lFlagBi = false;
					}
				}
				g++;
			}
			
			if (j == -1) { //optimality detected
				//ff << "optimal" << endl;
				//printResult(ff);
				feas = true;
				return;
			}

			VectorXd Aith = B.inverse().row(i) * A; //getting the ith row of B^-1 *A
			bool lFlag = true;
			int k = -1;
			double t = 0;
			double temp;
			if(lFlagBi){ //ratio test when the x_j leaves to its lower bound
				for (int kl = 0; kl < Li.size(); kl++) {
					if (Aith(Li[kl]) < -tol) {
						if (k == -1) {
							k = kl;
							t = cBar(Li[kl]) / -Aith(Li[kl]);
						}else{
							temp = cBar(Li[kl]) / -Aith(Li[kl]);
							if (temp < t) {
								t = temp;
								k = kl;
							}
						}

					}
				}

				for (int ku = 0; ku < Ui.size(); ku++) {
					if (Aith(Ui[ku]) > tol) {
						if (k == -1) {
							t = -cBar(Ui[ku]) / Aith(Ui[ku]);
							k = ku;
							lFlag = false;
						}else{
							temp = -cBar(Ui[ku]) / Aith(Ui[ku]);
							if (temp < t) {
								t = temp;
								k = ku;
								lFlag = false;
							}
						}
					}
				}
			}
			else { //ratio test when the x_j leaves to its upper bound
				for (int kl = 0; kl < Li.size(); kl++) {
						if (Aith(Li[kl]) > tol) {
							if (k == -1) {
								k = kl;
								t = cBar(Li[kl]) / Aith(Li[kl]);
							}else{
							temp = cBar(Li[kl]) / Aith(Li[kl]);
							if (temp < t) {
								t = temp;
								k = kl;
							}
						}
					}
				}

				for (int ku = 0; ku < Ui.size(); ku++) {
						if (Aith(Ui[ku]) < -tol) {
							if (k == -1) {
								t = cBar(Ui[ku]) / Aith(Ui[ku]);
								k = ku;
								lFlag = false;
							}else{
							temp = cBar(Ui[ku]) / Aith(Ui[ku]);
							if (temp < t) {
								t = temp;
								k = ku;
								lFlag = false;
							}
						}
					}
				}

			}
			
			if (k == -1) { //infeasible detected
				//ff << "infeasible" << endl;
				//printResult(ff);
				infeas = true;
				return;
			}

			if (lFlagBi) { //leaving variable
				Li.push_back(j);
				resultArray[j] = -1;
			}
			else{
				Ui.push_back(j);
				resultArray[j] = 1;
			}
			Bi.erase(Bi.begin() + i);

			if (lFlag) { //entering variable
				Bi.push_back(Li[k]);
				resultArray[Li[k]] = 0;
				Li.erase(Li.begin() + k);
			}
			else {
				Bi.push_back(Ui[k]);
				resultArray[Ui[k]] = 0;
				Ui.erase(Ui.begin() + k);
			}
		}
	}


	void printVec(vector<int> &origin) {
		for (int i = 0; i < origin.size(); i++) {
			cout << origin[i] << " ";
		}
		cout << endl;
	}

	void printVec2(vector<int> &origin, VectorXd &base) {
		for (int i = 0; i < origin.size(); i++) {
			cout << base(origin[i]) << " ";
		}
		cout << endl;
	}


	void print() { //for test
		cout << "c:" << endl;
		cout << c << endl << endl;
		/*cout << "l:" << endl;
		cout << l << endl << endl;
		cout << "u:" << endl;
		cout << u << endl << endl;
		cout << "A:" << endl;
		cout << A << endl << endl;
		cout << "B:" << endl;
		cout << B << endl << endl;
		cout << "b:" << endl;
		cout << b << endl << endl;*/
		cout << "Bi:" << endl;
		printVec(Bi);
		cout << endl;
		cout << "cBar:" << endl;
		cout << cBar << endl << endl;
		cout << "Li:" << endl;
		printVec(Li);
		cout << endl;
		cout << "Ui:" << endl;
		printVec(Ui);
		cout << endl;
		cout << "L:" << endl;
		cout << L << endl << endl;
		cout << "U:" << endl;
		cout << U << endl << endl;
		cout << "xBbar:" << endl;
		cout << xb << endl<< endl;
		cout << "resultX:" << endl;
		cout << resultX << endl << endl;
		//cout << feas << endl;
	}
};

class branchNBound {
	int n;
	VectorXd x;
	VectorXd resultX;
	double tol;
	double z;
	bool feas = false;
	
	int counter = 0;

public:
	branchNBound(int n, double tol) :n{ n }, tol{ tol } {
		z = INFINITY;
		x = VectorXd(n);
		resultX = VectorXd(n);
	}

	void printResult(ostream &ff) {
		if (feas) {
			ff << "optimal";
			ff << " # objective: "<< z << endl;
			for (int i = 0; i < n; i++) {
				ff << resultX(i) << " ";
			}
		}
		else {
			ff << "infeasible" << endl;
		}
	}

	void branchAndBound(dualSimplex &lp) {
		if (lp.isInfeasible()) {
			return;
		}
		double obj = lp.calculateObj();
		x = lp.getResultX();
		int pos = lp.getNonint();

		if (pos == -1) {
			if (obj < z - tol) {
				feas = true;
				z = obj;
				resultX = lp.getResultX();
			}
		}
		else {
			double low = floor(x(pos));
			double high = ceil(x(pos));
			if (lp.getUpperBound(pos) > low + tol) {
				dualSimplex lp0{ lp };
				lp0.changeUpperBound(pos, low);
				lp0.dualSimplexMethod();
				if (lp0.isFeasible()) {
					if (lp0.calculateObj() < z - tol) {
						branchAndBound(lp0);
					}
				}
			}

			if (lp.getLowerBound(pos) < high - tol) {
				dualSimplex lp1{ lp };
				lp1.changeLowerBound(pos, high);
				lp1.dualSimplexMethod();
				if (lp1.isFeasible()) {
					if (lp1.calculateObj() < z - tol) {
						branchAndBound(lp1);
					}
				}
			}
		}
	}

};

int main(int argc, char *argv[])
{
	string inputfile;
	string outputfile;
	if (argc != 2) {
		cout << "wrong number of arguments" << endl;
	}
	else {
	inputfile = argv[1];
	outputfile = argv[2]; //"D:/dev/simplexmethod/output.txt";
	ifstream f(inputfile);
	string tempinput;

	getline(f, tempinput);
	stringstream ss(tempinput);
	int m, n;
	ss >> m;
	ss >> n;
	double tol = pow(10, -6);
	dualSimplex dsm{ m,n,tol };
	branchNBound bb{ n, tol };
	for (int i = 1; i < 4; i++) {
		getline(f, tempinput);
		stringstream ss(tempinput);
		for (int j = 0; j < n; j++) {
			double tmp;
			ss >> tmp;
			dsm.addToVec(tmp, i);
		}
	}

	for (int i = 0; i < m; i++) {
		getline(f, tempinput);
		stringstream ss(tempinput);
		for (int j = 0; j < n; j++) {
			double tmp;
			ss >> tmp;
			dsm.addToMat(tmp, i, j);
		}
	}

	getline(f, tempinput);
	stringstream ss1(tempinput);
	for (int j = 0; j < m; j++) {
		double tmp;
		ss1 >> tmp;
		dsm.addToVec(tmp, 4);
	}
	dsm.formulateProblem();
	dsm.dualSimplexMethod();
	bb.branchAndBound(dsm);
	ofstream ff(outputfile);
	bb.printResult(ff);
	}
	return 0;
}