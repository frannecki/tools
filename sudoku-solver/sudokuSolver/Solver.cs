using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace sudokuSolver
{
    /// <summary>
    /// Solve the Sudoku puzzle
    /// </summary>
    public class Solver
    {
        private List<List<int>> data;
        List<int> rowSt, colSt, latSt;
        List<cellPos> cellsToFill;
        private int s;  // size of sudoku grid

        public Solver(ref ViewModel vm, out bool success)
        {
            data = new List<List<int>>();
            rowSt = new List<int>();
            colSt = new List<int>();
            latSt = new List<int>();
            cellsToFill = new List<cellPos>();
            s = vm.rowData.Count;
            for (int i = 0; i < s; ++i)
            {
                rowSt.Add(0);
                colSt.Add(0);
                latSt.Add(0);
            }
            for (int i = 0; i < s; ++i)
            {
                data.Add(new List<int>());
                foreach(var item in vm.rowData[i].COLNUM)
                {
                    data[i].Add(getDigit(item.NUM));
                }
                for (int j = 0; j < s; ++j)
                {
                    if(data[i][j] == -1)
                    {
                        cellsToFill.Add(new cellPos() { rowidx = i, colidx = j });
                    }
                    else
                    {
                        vm.rowData[i].COLNUM[j].ISANS = false;  // filled elements

                        int cmp = 1 << (data[i][j] - 1);
                        int latidx = (i / 3) * 3 + j / 3;
                        bool rowConflict = (rowSt[i] & cmp) != 0;
                        bool colConflict = (colSt[j] & cmp) != 0;
                        bool latConflict = (latSt[latidx] & cmp) != 0;
                        if (rowConflict || colConflict || latConflict)
                        {
                            success = false;
                            return;
                        }
                        else
                        {
                            rowSt[i] |= cmp;
                            colSt[j] |= cmp;
                            latSt[latidx] |= cmp;
                        }
                    }
                }
            }
            List<int> status = new List<int>();
            for(int i = 0; i < cellsToFill.Count; ++i)
            {
                status.Add(0);
            }
            success = solveSudoku(ref status, 0);
            if (success)
            {
                for (int i = 0; i < cellsToFill.Count; ++i)
                {
                    int row = cellsToFill[i].rowidx;
                    int col = cellsToFill[i].colidx;
                    vm.rowData[row].COLNUM[col].NUM = ToStr(status[i]);
                    vm.rowData[row].COLNUM[col].ISANS = true;  // calculated elements
                }
            }
        }

        private bool solveSudoku(ref List<int> status, int prog)
        {
            if (prog == cellsToFill.Count)
            {
                int r, c;
                for (int i = 0; i < cellsToFill.Count; ++i)
                {
                    r = cellsToFill[i].rowidx;
                    c = cellsToFill[i].colidx;
                    data[r][c] = status[i];
                }
                return true;
            }
            int row = cellsToFill[prog].rowidx;
            int col = cellsToFill[prog].colidx;
            int latidx = (row / 3) * 3 + col / 3;
            for (int i = 1; i <= s; ++i)
            {
                int cmp = 1 << (i - 1);
                bool rowConflict = ((rowSt[row] & cmp) != 0);
                bool colConflict = ((colSt[col] & cmp) != 0);
                bool latConflict = ((latSt[latidx] & cmp) != 0);
                if (rowConflict || colConflict || latConflict) continue;
                rowSt[row] |= cmp;
                colSt[col] |= cmp;
                latSt[latidx] |= cmp;
                status[prog] = i;
                if (solveSudoku(ref status, prog + 1)) return true;
                rowSt[row] ^= cmp;
                colSt[col] ^= cmp;
                latSt[latidx] ^= cmp;
            }
            return false;
        }

        public int getDigit(string str)
        {
            int d;
            bool digit = false;
            if(Int32.TryParse(str, out d))
            {
                digit = (d >= 1 && d <= s);
            }
            if (digit) return d;
            else return -1;
        }

        public string ToStr(int num)
        {
            if (num >= 1 && num <= s) return num.ToString();
            else return "";
        }
    }

    public class cellPos
    {
        public int rowidx { get; set; }
        public int colidx { get; set; }
    }
}
