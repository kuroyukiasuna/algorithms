if (lFlagBi) {
	for (int kl = 0; kl < Li.size(); kl++) {
		double Aith = B.inverse().row(i) * A.col(Li[kl]);
		if (Aith < -tol) {
			if (kl == 0) {
				k = 0;
				t = cBar(Li[kl]) / -Aith;
			}
			else {
				temp = cBar(Li[kl]) / -Aith;
				if (temp < t) {
					t = temp;
					k = kl;
				}
			}
		}
	}

	for (int ku = 0; ku < Ui.size(); ku++) {
		double Aith = B.inverse().row(i) * A.col(Ui[ku]);
		if (Aith > tol) {
			if (k == -1) {
				t = -cBar(Ui[ku]) / Aith;
				k = ku;
				lFlag = false;
			}
			else {
				temp = -cBar(Ui[ku]) / Aith;
				if (temp < t) {
					t = temp;
					k = ku;
					lFlag = false;
				}
			}
		}
	}
}
else {
	for (int kl = 0; kl < Li.size(); kl++) {
		double Aith = B.inverse().row(i) * A.col(Li[kl]);
		if (Aith > tol) {
			if (kl == 0) {
				k = 0;
				t = cBar(Li[kl]) / Aith;
			}
			else {
				temp = cBar(Li[kl]) / Aith;
				if (temp < t) {
					t = temp;
					k = kl;
				}
			}
		}
	}

	for (int ku = 0; ku < Ui.size(); ku++) {
		double Aith = B.inverse().row(i) * A.col(Ui[ku]);
		if (Aith < -tol) {
			if (k == -1) {
				t = cBar(Ui[ku]) / Aith;
				k = ku;
				lFlag = false;
			}
			else {
				temp = cBar(Ui[ku]) / Aith;
				if (temp < t) {
					t = temp;
					k = ku;
					lFlag = false;
				}
			}
		}
	}

}


if (lFlagBi) {
	for (int kl = 0; kl < Li.size(); kl++) {
		if (Aith(Li[kl]) < -tol) {
			if (kl == 0) {
				k = 0;
				t = cBar(Li[kl]) / -Aith(Li[kl]);
			}
			else {
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
			}
			else {
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
else {
	for (int kl = 0; kl < Li.size(); kl++) {
		if (Aith(Li[kl]) > tol) {
			if (kl == 0) {
				k = 0;
				t = cBar(Li[kl]) / Aith(Li[kl]);
			}
			else {
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
			}
			else {
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



//VectorXd Aith = B.inverse().row(i) * A; //getting the ith row of B^-1 *A
bool lFlag = true;
int k = -1;
double t = 0;
double temp;
if (lFlagBi) { //ratio test when the x_j leaves to its lower bound
	for (int kl = 0; kl < Li.size(); kl++) {
		double Aith = B.inverse().row(i) * A.col(Li[kl]);
		if (Aith < -tol) {
			if (k == -1) {
				k = kl;
				t = cBar(Li[kl]) / -Aith;
			}
			else {
				temp = cBar(Li[kl]) / -Aith;
				if (temp < t) {
					t = temp;
					k = kl;
				}
			}

		}
	}

	for (int ku = 0; ku < Ui.size(); ku++) {
		double Aith = B.inverse().row(i) * A.col(Ui[ku]);
		if (Aith > tol) {
			if (k == -1) {
				t = -cBar(Ui[ku]) / Aith;
				k = ku;
				lFlag = false;
			}
			else {
				temp = -cBar(Ui[ku]) / Aith;
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
		double Aith = B.inverse().row(i) * A.col(Li[kl]);
		if (Aith > tol) {
			if (k == -1) {
				k = kl;
				t = cBar(Li[kl]) / Aith;
			}
			else {
				temp = cBar(Li[kl]) / Aith;
				if (temp < t) {
					t = temp;
					k = kl;
				}
			}
		}
	}

	for (int ku = 0; ku < Ui.size(); ku++) {
		double Aith = B.inverse().row(i) * A.col(Ui[ku]);
		if (Aith < -tol) {
			if (k == -1) {
				t = cBar(Ui[ku]) / Aith;
				k = ku;
				lFlag = false;
			}
			else {
				temp = cBar(Ui[ku]) / Aith;
				if (temp < t) {
					t = temp;
					k = ku;
					lFlag = false;
				}
			}
		}
	}

}