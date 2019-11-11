using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;

namespace sudokuSolver
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private ViewModel vm;
        public MainWindow()
        {
            InitializeComponent();
            vm = new ViewModel() { buttonColor = "Azure" };
            DataContext = vm;
        }

        public void quit_menu_click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        public void about_menu_click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("9x9数独作弊", "关于", MessageBoxButton.OK);
        }

        public void solve_button_click(object sender, RoutedEventArgs e)
        {
            bool success;
            Solver solver = new Solver(ref vm, out success);
            vm.buttonColor = "Pink";
            if (!success) MessageBox.Show("无解！", "警告", MessageBoxButton.OK);
            undo_button.IsEnabled = true;
        }

        public void reset_button_click(object sender, RoutedEventArgs e)
        {
            for(int i = 0; i < 9; ++i)
            {
                vm.rowData[i].setAllValues("");
            }
            vm.buttonColor = "Azure";
        }

        public void undo_button_click(object sender, RoutedEventArgs e)
        {
            foreach(var row in vm.rowData)
            {
                foreach(var item in row.COLNUM)
                {
                    if(item.ISANS)  item.NUM = "";
                }
            }
            undo_button.IsEnabled = false;
        }
    }
}