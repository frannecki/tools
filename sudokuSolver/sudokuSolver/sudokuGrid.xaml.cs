using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace sudokuSolver
{
    /// <summary>
    /// Interaction logic for sudokuGrid.xaml
    /// </summary>
    public partial class sudokuGrid : UserControl
    {
        public sudokuGrid()
        {
            InitializeComponent();
        }

        public void textbox_preview_keydown(object sender, KeyEventArgs e)
        {
            // only characters [1-9], delete, backspace, left and right are allowed
            bool shift_pressed = (Keyboard.Modifiers & ModifierKeys.Shift) != 0;
            TextBox _currentbox = e.Source as TextBox;  // get event source element (textbox)
            //TextBox _currentbox = Keyboard.FocusedElement as TextBox;  // get currently focused element (textbox)
            if (shift_pressed)  e.Handled = true;
            else if (_currentbox.Text.Length >= 1)
            {
                bool inputManip = (e.Key == Key.Delete) || (e.Key == Key.Back) || (e.Key == Key.Left) || (e.Key == Key.Right);
                if(!inputManip)  e.Handled = true;
            }
            else
            {
                bool isValid = (e.Key >= Key.D1 && e.Key <= Key.D9) || (e.Key >= Key.NumPad1 && e.Key <= Key.NumPad9)
                    || (e.Key == Key.Delete) || (e.Key == Key.Back) || (e.Key == Key.Left) || (e.Key == Key.Right);
                if (!isValid) e.Handled = true;
            }
        }
    }
}
