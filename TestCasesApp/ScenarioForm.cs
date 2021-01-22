using FileHelpers;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TestCasesApp
{
  public partial class ScenarioForm : Form
  {
    private BindingList<TestCase> cases = new BindingList<TestCase>();
    private readonly Instrument _instrument;
    /// <summary>
    /// 
    /// </summary>
    /// <param name="instrument"></param>
    public ScenarioForm(Instrument instrument)
    {
      _instrument = instrument;
      //---
      InitializeComponent();

      dataGridView1.AutoGenerateColumns = false;
      //----
      dataGridView1.DataSource = cases;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void ScenarioForm_Load(object sender, EventArgs e)
    {
      folderBox.Text = Properties.Settings.Default.scenarioFolder;
      //--- checks
      if (_instrument == null)
        return;
      //---
      symbolText.Text = _instrument.Symbol;
      //--- clear
      cases.Clear();
      //---
      engine.ErrorMode = ErrorMode.SaveAndContinue;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void opListBox_ItemCheck(object sender, ItemCheckEventArgs e)
    {
      //--- checks
      if (e.Index < 0)
        return;
      //---
      if (e.NewValue == CheckState.Checked)
      {
        cases.Add(new TestCase()
        {
          CaseName = textScenario.Text,
          Command = "OrderSend",
          Lots = lotsUpDown.Value.ToString("F2"),
          Symbol = symbolText.Text.Trim(),
          Type = opListBox.Items[e.Index].ToString(),
          TP = tpUpDown.Value.ToString("F0"),
          SL = slUpDown.Value.ToString("F0"),
          Price="0",
          Slippage = slippageUpDown.Value.ToString("F0")
        });
      }
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void btnCreate_Click(object sender, EventArgs e)
    {
      //--- cehcks
      if (string.IsNullOrEmpty(folderBox?.Text))
        return;

      string file = $"{folderBox.Text}\\commandfile.txt";
      //--- write to file
      engine.AppendToFile(file, cases.ToList());
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void folderBox_TextChanged(object sender, EventArgs e)
    {
      Properties.Settings.Default.scenarioFolder = folderBox.Text;
      //---
      Properties.Settings.Default.Save();
    }
    /// <summary>
    /// 
    /// </summary>
    private FileHelperEngine<TestCase> engine = new FileHelperEngine<TestCase>();
  }
}
