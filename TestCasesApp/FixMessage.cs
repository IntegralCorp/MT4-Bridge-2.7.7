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
	public partial class FixMessage: Form
	{
		private readonly QuickFix.DataDictionary.DataDictionary dataDictionary=new QuickFix.DataDictionary.DataDictionary("FIX43.xml");

		private readonly BindingList<CFixData> _values;
		public FixMessage(BindingList<CFixData> values)
		{
			_values=values;

			InitializeComponent();

			dataGridView1.DataSource=_values;
		}
	}
}
