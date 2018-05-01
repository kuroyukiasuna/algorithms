# algorithms
Implemented algorithms in c++

1.BBSimplexMethod:</br>
  &nbsp;&nbsp;1.)Dual Simplex Method for solving linear systems (in this case use the simplexmethod(working version).cpp file)</br></br>
  &nbsp;&nbsp;2.)Branch and Bound Method for solving integer systems (default)</br>
  
  Input format(See examples in Data folders):</br>
  </br>
  m n</br>
  c1 ... cn (obj function value)</br>
  l1 ... ln (lowerbound) </br>
  u1 ... un (upperbound) </br>
  a(0,0) ... a(0,n-1)    </br>
  :&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:        </br>
  .&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;.        </br>
  a(m-1,0) ... a(m-1, n-1) (the constraint matrix, with mxn dimensions) </br>
  b1 ... bn    (constraint) </br>
  
  Note: </br>
  1.External library <a href="http://eigen.tuxfamily.org/index.php?title=Main_Page">Eigen</a> is required for this algorithm</br>
  2.The algorihm correctly handles floating point inaccuracies with a tolerance value initially set at 10^-6</br>
  3.For B&B algorithm, prunning and warm-start are effectively implemented</br>
