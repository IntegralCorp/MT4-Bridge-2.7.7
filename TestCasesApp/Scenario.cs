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
  public partial class Scenario : Form
  {
    private BindingList<Instrument> _instruments;
    /// <summary>
    /// 
    /// </summary>
    /// <param name="instruments"></param>
    public Scenario(BindingList<Instrument> instruments)
    {
      _instruments = instruments;
      //---
      InitializeComponent();

      //----
      dataGridView1.AutoGenerateColumns = true;
      //---
      dataGridView1.DataSource = _instruments;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void btnOrders_Click(object sender, EventArgs e)
    {
      //--- checks
      if (string.IsNullOrEmpty(folderBox?.Text))
        return;
      //---
      var list = InstrumentsEngine.Instance.Execute(folderBox.Text);
      foreach (var inst in list)
        _instruments.Add(inst);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void update_Click(object sender, EventArgs e)
    {
      //--- checks
      if (string.IsNullOrEmpty(folderBox?.Text))
        return;
      //---
      InstrumentsEngine.Instance.Update(folderBox.Text, _instruments.ToList());
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void contextMenuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
    {
      if(e.ClickedItem.Text=="Add")
      {
        var instrument = dataGridView1.SelectedRows[0].DataBoundItem as Instrument;
        ScenarioForm form = new ScenarioForm(instrument);
        form.Show();
      }
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void Scenario_Load(object sender, EventArgs e)
    {
       folderBox.Text=Properties.Settings.Default.instruments_folder;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void folderBox_TextChanged(object sender, EventArgs e)
    {
      Properties.Settings.Default.instruments_folder= folderBox.Text;
      //---
      Properties.Settings.Default.Save();
    }
  }
}
