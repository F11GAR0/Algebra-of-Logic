using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AlLogicGUI
{
    public partial class MainForm : Form
    {
        [DllImport("al_logic.dll", CallingConvention = CallingConvention.StdCall)]
        static extern int Calculate(String expr, String[] variables, int[] variables_values, int variables_count);

        int[][] ONE_VARS_VALUES_TABLE = new int[][]
        {
            new int[]{ 0 },
            new int[]{ 1 },
        };

        int[][] TWO_VARS_VALUES_TABLE = new int[][]
        {
            new int[]{ 0, 0},
            new int[]{ 0, 1},
            new int[]{ 1, 0},
            new int[]{ 1, 1}
        };

        int[][] THREE_VARS_VALUES_TABLE = new int[][]
        {
            new int[]{ 0, 0, 0},
            new int[]{ 0, 0, 1},
            new int[]{ 0, 1, 0},
            new int[]{ 0, 1, 1},
            new int[]{ 1, 0, 0},
            new int[]{ 1, 0, 1},
            new int[]{ 1, 1, 0},
            new int[]{ 1, 1, 1}
        };

        int[][] FOUR_VARS_VALUES_TABLE = new int[][]
        {
            new int[]{ 0, 0, 0, 0},
            new int[]{ 0, 0, 0, 1},
            new int[]{ 0, 0, 1, 0},
            new int[]{ 0, 0, 1, 1},
            new int[]{ 0, 1, 0, 0},
            new int[]{ 0, 1, 0, 1},
            new int[]{ 0, 1, 1, 0},
            new int[]{ 0, 1, 1, 1},
            new int[]{ 1, 0, 0, 0},
            new int[]{ 1, 0, 0, 1},
            new int[]{ 1, 0, 1, 0},
            new int[]{ 1, 0, 1, 1},
            new int[]{ 1, 1, 0, 0},
            new int[]{ 1, 1, 0, 1},
            new int[]{ 1, 1, 1, 0},
            new int[]{ 1, 1, 1, 1}
        };

        public MainForm()
        {
            InitializeComponent();
        }
        List<string> GetVariablesFromExpression(string text)
        {
            string[] splitted = text.Split(new char[] { '=', '+', '>', '|', '&', '!', '(', ')' });

            if (splitted.Length > 0)
            {
                List<string> variables = new List<string>();
                for (int i = 0, len = splitted.Length; i < len; i++)
                {
                    if (!variables.Contains(splitted[i]) && splitted[i].Length > 0)
                    {
                        variables.Add(splitted[i]);
                    }
                }
                return variables;
            } else
                return new List<string>();
        }
        private void tbExpression_TextChanged(object sender, EventArgs e)
        {
            string text = tbExpression.Text.Replace(' ', '\0');
            text = text.Replace('\n', '\0');
            text = text.Replace('\r', '\0');
            //detect variables
            lbVariables.Items.Clear();
            lbVariables.Items.AddRange(GetVariablesFromExpression(text).ToArray());
        }

        private void bCalc_Click(object sender, EventArgs e)
        {
            tbOutput.Text = "";
            string text = tbExpression.Text.Replace(' ', '\0');
            text = text.Replace('\n', '\0');
            text = text.Replace('\r', '\0');
            var variables = GetVariablesFromExpression(text);

            for(int i = 0; i < variables.Count; i++)
            {
                tbOutput.Text += variables[i] + " ";
            }
            tbOutput.Text += " RES" + Environment.NewLine;
            
            for(int i = 0, len = (int)Math.Pow(2, variables.Count); i < len; i++)
            {
                int[] values = new int[variables.Count];
                switch (variables.Count)
                {
                    case 1:
                        values = ONE_VARS_VALUES_TABLE[i];
                        break;
                    case 2:
                        values = TWO_VARS_VALUES_TABLE[i];
                        break;
                    case 3:
                        values = THREE_VARS_VALUES_TABLE[i];
                        break;
                    case 4:
                        values = FOUR_VARS_VALUES_TABLE[i];
                        break;
                }

                string values_str = "";
                for(int j = 0; j < values.Length; j++)
                {
                    values_str += values[j] + " ";
                }
                tbOutput.Text += values_str + Calculate(tbExpression.Text, variables.ToArray(), values, variables.Count).ToString() + Environment.NewLine;
            }
            
        }
    }
}
