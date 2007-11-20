package confdb.gui;

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;


/**
 * GenericTransferable.java
 * 
 * <p>This transferable takes an object as data that is to be
 * transferred. It uses DataFlavor.stringFlavor, which is supported by
 * all objects. This transferable can be used in cases where a special
 * handling in terms of which data flavors are acceptable or which
 * data is transported do not matter.</p>
 * 
 * @author Ulrich Hilger
 * @author Light Development
 * @author <a href="http://www.lightdev.com">http://www.lightdev.com</a>
 * @author <a href="mailto:info@lightdev.com">info@lightdev.com</a>
 * @author published under the terms and conditions of the
 *      GNU General Public License,
 *      for details see file gpl.txt in the distribution
 *      package of this software
 *
 * @version 1, 30.07.2005
 */
public class GenericTransferable implements Transferable
{
    //
    // member data
    //

    /** the data this transferable transports */
    private Object data;
    
    /** storage for data flavors supported of this transferable */
    private static final DataFlavor[] flavors = new DataFlavor[1];
    
    /** the actual flavors supported by this transferable */
    static { flavors[0] = DataFlavor.stringFlavor; }


    //
    // construction
    //
    
    /** standard constructor */
       public GenericTransferable(Object data)
    {
	super();
	this.data = data;
	System.out.println("mimeType = " + flavors[0].getMimeType());
    }
    

    //
    // member functions
    //

    /** get the data flavors supported by this object */
    public DataFlavor[] getTransferDataFlavors() { return flavors; }
    
    /**determine whether or not a given data flavor is supported */
    public boolean isDataFlavorSupported(DataFlavor flavor) { return true; }

    /**get the data this transferable transports */
    public Object getTransferData(DataFlavor flavor)
	throws UnsupportedFlavorException, IOException { return data; }
    
}
