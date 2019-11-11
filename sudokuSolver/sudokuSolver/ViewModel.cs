using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace sudokuSolver
{
    /// <summary>
    /// Manipulates data in listbox
    /// </summary>
    public class ViewModel : INotifyPropertyChanged
    {
        public ViewModel()
        {
            for (int i = 0; i < 9; ++i)
            {
                rowData.Add(new SudokuDataRow(""));
            }
        }

        public static readonly DependencyProperty buttonColorProperty =
            DependencyProperty.Register("buttonColor", typeof(string), typeof(MainWindow), new FrameworkPropertyMetadata("buttonColor"));
        public string bcolor;   // background color of reset button
        public string buttonColor
        {
            get { return (string)bcolor; }
            set { bcolor = value; OnPropertyChanged("buttonColor"); }
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private ObservableCollection<SudokuDataRow> data = new ObservableCollection<SudokuDataRow>();
        public ObservableCollection<SudokuDataRow> rowData
        {
            get { return data; }
            set
            {
                if (data != null)
                {
                    foreach (var row in data)
                    {
                        row.PropertyChanged -= PropertyChanged;
                    }
                }
                if (value != null)
                {
                    foreach (var item in value)
                    {
                        item.PropertyChanged += PropertyChanged;
                    }
                }
                data = value;
                OnPropertyChanged("rowData");
            }
        }

        protected virtual void OnPropertyChanged(string caller)
        {
            var handler = PropertyChanged;
            handler?.Invoke(this, new PropertyChangedEventArgs(caller));
        }
    }


    /// <summary>
    /// Manipulates each row (listbox) of data in listbox
    /// </summary>
    public class SudokuDataRow : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        ObservableCollection<cellNum> colnum;
        public ObservableCollection<cellNum> COLNUM
        {
            get { return colnum; }
            set
            {
                if(colnum != null)
                {
                    foreach(var item in colnum)
                    {
                        item.PropertyChanged -= PropertyChanged;
                    }
                }
                if(value != null)
                {
                    foreach(var item in value)
                    {
                        item.PropertyChanged += PropertyChanged;
                    }
                }
                colnum = value;
                OnPropertyChanged("COLNUM");
            }
        }

        public SudokuDataRow(string initial)
        {
            colnum = new ObservableCollection<cellNum>();
            for(int i = 0; i < 9; ++i)
            {
                colnum.Add(new cellNum());
            }
            setAllValues(initial);
        }

        protected virtual void OnPropertyChanged(string info)
        {
            var handler = PropertyChanged;
            handler?.Invoke(this, new PropertyChangedEventArgs(info));
        }

        public void setAllValues(string val)
        {
            foreach(var item in COLNUM)
            {
                item.NUM = val;
            }
        }
    }


    /// <summary>
    /// Manipulates each cell of data in listbox
    /// </summary>
    public class cellNum: INotifyPropertyChanged
    {
        private List<string> colors, bcolors;
        public event PropertyChangedEventHandler PropertyChanged;
        string num, color, bcolor;
        bool isAns;

        public cellNum()
        {
            isAns = false;

            colors = new List<string>();
            colors.Add("MidnightBlue"); colors.Add("SlateGray"); colors.Add("Firebrick");
            colors.Add("Indigo"); colors.Add("LightSeaGreen"); colors.Add("LightCoral");
            colors.Add("Peru"); colors.Add("Magenta"); colors.Add("Purple");

            bcolors = new List<string>();
            bcolors.Add("Azure"); bcolors.Add("SeaShell"); bcolors.Add("LawnGreen");
            bcolors.Add("Khaki"); bcolors.Add("LightCyan"); bcolors.Add("Lavender");
            bcolors.Add("Pink"); bcolors.Add("Tan"); bcolors.Add("Wheat");
        }

        public string NUM
        {
            get { return (string)num; }
            set
            {
                num = value;
                OnPropertyChanged("NUM");
                COLOR = getcolor(num);
                BCOLOR = getcolor(num, true);
            }
        }

        public string COLOR
        {
            get { return (string)color; }
            set
            {
                color = value;
                OnPropertyChanged("COLOR");
            }
        }

        public string BCOLOR
        {
            get { return (string)bcolor; }
            set
            {
                bcolor = value;
                OnPropertyChanged("BCOLOR");
            }
        }

        public bool ISANS
        {
            get { return isAns; }
            set
            {
                isAns = value;
                if (isAns) BCOLOR = "Transparent";
                else BCOLOR = getcolor(NUM, true);
            }
        }

        public void OnPropertyChanged(string caller)
        {
            var handler = PropertyChanged;
            handler?.Invoke(this, new PropertyChangedEventArgs(caller));
        }

        private string getcolor(string num, bool isbg = false)
        {
            int d = 0;
            bool digit = Int32.TryParse(num, out d);
            if (!isbg)
            {
                if (digit && (1 <= d && d <= 9)) return colors[d-1];
                else return "Black";
            }
            else
            {
                if (digit && (1 <= d && d <= 9)) return bcolors[d-1];
                else return "White";
            }
        }
    }
}
