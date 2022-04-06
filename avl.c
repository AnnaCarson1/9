#include "DS.h"
int min ;

int height_p ( Elm *p )
{
        if ( p == NULL )
            return 0 ;
        else
            return p -> height ;
}

int balance_p ( Elm *p )
{
        if ( p == NULL )
            return 0 ;
        return ( height_p ( p -> L ) - height_p ( p -> R ) ) ;
}

Elm *right_rotation ( Elm *p )
{
        struct Elm *k = p -> L ;
        struct Elm *temp = k -> R ;
        k -> R = p ;
        p -> L = temp ;
        if ( height_p ( p -> L ) > height_p ( p -> R ) )
            p -> height = height_p ( p -> L ) + 1 ;
        else
            p -> height = height_p ( p -> R ) + 1 ;
        if ( height_p ( k -> L ) > height_p ( k -> R ) )
            k -> height = height_p ( k -> L ) + 1 ;
        else
            k -> height = height_p ( k -> R ) + 1 ;
        p -> len = size_p ( p -> L ) + size_p ( p -> R ) + 1 ;
        k -> len = size_p ( k -> L ) + size_p ( k -> R ) + 1 ;
        return k ;
}

Elm *left_rotation ( Elm *p )
{
        struct Elm *k = p -> R ;
        struct Elm *temp = k -> L ;
        k -> L = p ;
        p -> R = temp ;
        if ( height_p ( p -> L ) > height_p ( p -> R ) )
            p -> height = height_p ( p -> L ) + 1 ;
        else
            p -> height = height_p ( p -> R ) + 1 ;
        if ( height_p ( k -> L ) > height_p ( k -> R ) )
            k -> height = height_p ( k -> L ) + 1 ;
        else
            k -> height = height_p ( k -> R ) + 1 ;
        p -> len = size_p ( p -> L ) + size_p ( p -> R ) + 1 ;
        k -> len = size_p ( k -> L ) + size_p ( k -> R ) + 1 ;
        return k ;
}

Elm *insert_x ( Elm *p , int x )
{
        if ( p == NULL ) {
            struct Elm *k = malloc ( sizeof ( Elm ) ) ;
            k -> L = NULL ;
            k -> R = NULL ;
            k -> x = x ;
            k -> len = 1 ;
            k -> height = 1 ;
            return k ;
        }
        if ( p -> x < x ) {
            p -> R = insert_x ( p -> R , x ) ;
            p -> len ++ ;
        }
        else {
            p -> L = insert_x ( p -> L , x ) ;
            p -> len ++ ;
        }
        p -> len = size_p ( p -> L ) + size_p ( p -> R ) + 1 ;
        if ( height_p ( p -> L ) > height_p ( p -> R ) )
            p -> height = height_p ( p -> L ) + 1 ;
        else
            p -> height = height_p ( p -> R ) + 1 ;
        int balance = balance_p ( p ) ;
        if ( balance > 1 && p -> L -> x > x )
            return right_rotation ( p ) ;
        if ( balance < -1 && p -> R -> x < x )
            return left_rotation ( p ) ;
        if ( balance > 1 && p -> L -> x < x ) {
            p -> L = left_rotation ( p -> L ) ;
            return right_rotation ( p ) ;
        }
        if ( balance < -1 && p -> R -> x > x ) {
            p -> R = right_rotation ( p -> R ) ;
            return left_rotation ( p ) ;
        }
        return p ;
}

Elm *search_x ( Elm *p , int x )
{
        if ( p == NULL ) {
            return NULL ;
        }
        if ( p -> x == x ) {
            return p ;
        }
        else
            if ( p -> x > x ) {
                return search_x ( p -> L , x ) ;
            }
            else {
                return search_x ( p -> R , x ) ;
            }
}

Elm *min_val ( Elm *p )
{
        Elm *temp = p ;
        if ( p -> L == NULL )
            min = p -> x ;
        else
            min = p -> L -> x ;
        while ( temp != NULL && temp -> L != NULL ) {
            temp = temp -> L ;
            min = temp -> x ;
        }
        return temp ;
}

Elm *delete_x ( Elm *p , int x )
{
        if ( p == NULL )
            return NULL ;
        if ( p -> x > x )
            p -> L = delete_x ( p -> L , x ) ;
        else
            if ( p -> x < x )
                p -> R = delete_x ( p -> R , x ) ;
            else {
                if ( p -> L == NULL ) {
                    Elm *temp = p -> R ;
                    free ( p ) ;
                    return temp ;
                }
                else
                    if ( p -> R == NULL ) {
                        Elm *temp = p -> L ;
                        free ( p ) ;
                        return temp ;
                    }
                Elm *temp = min_val ( p -> R ) ;
                p -> x = temp -> x ;
                p -> R = delete_x ( p -> R , temp -> x ) ;
            }
        if ( p == NULL )
            return NULL ;
        p -> len = size_p ( p -> L ) + size_p ( p -> R ) + 1 ;
        if ( height_p ( p -> L ) > height_p ( p -> R ) )
            p -> height = height_p ( p -> L ) + 1 ;
        else
            p -> height = height_p ( p -> R ) + 1 ;
        int balance = balance_p ( p ) ;
        if ( balance > 1 && balance_p ( p -> L ) >= 0 )
            return right_rotation ( p ) ;
        if ( balance > 1 && balance_p ( p -> L ) < 0 ) {
            p -> L =  left_rotation ( p -> L ) ;
            return right_rotation ( p ) ;
        }
        if ( balance < -1 && balance_p ( p -> R ) <= 0 )
            return left_rotation ( p ) ;
        if ( balance < -1 && balance_p (p -> R ) > 0 ) {
            p -> R = right_rotation ( p -> R ) ;
            return left_rotation ( p ) ;
        }
        return p ;
}

int size_p ( Elm *p )
{
        if ( p == NULL )
            return 0 ;
        else
            return p -> len ;
}

void in_order_p ( Elm *p )
{
        if ( p != NULL ) {
            in_order_p ( p -> L ) ;
            printf ( "%d %d %d\n" , p -> x , size_p ( p ) , height_p ( p ) ) ;
            in_order_p ( p -> R ) ;
        }
}

/*
  `ptree`-ийн зааж байгаа модонд `x` утгыг оруулна.
  Оруулахдаа хоёртын хайлтын модны зарчмаар оруулах бөгөөд оруулсан
  байрлалаас дээшхи өвөг эцгийн `len`, `height` утгууд өөрчлөгдөнө.
  Мод хоосон байсан бол `ptree->root` хаяг өөрчлөгдөж шинээр орсон оройг заана.
  Хэрэв мод тэнцвэрээ алдсан бол тохирох тэнцвэржүүлэх үйлдлүүдийг хийнэ.
 */
void avl_put(AVL *ptree, int x)
{
        ptree -> root = insert_x ( ptree -> root , x ) ;
}

/*
  `ptree`-ийн зааж байгаа модноос `x` утгыг хайн олдсон оройн `Elm*` хаягийг буцаана.
  Олдохгүй бол `NULL` хаягийг буцаана.
  Мод дандаа ялгаатай элементүүд хадгална гэж үзэж болно.
 */
Elm *avl_get(const AVL *ptree, int x)
{
        struct Elm *temp = search_x ( ptree -> root , x ) ;
        if ( temp == NULL )
            return NULL ;
        temp -> len = size_p ( temp ) ;
        temp -> height = height_p ( temp ) ;
        return temp ;
}

/*
  Устгах функц: ТМноос `x` утгыг хайж олоод устгана.
  Олдохгүй бол юу ч хийхгүй.
  Хэрэв мод тэнцвэрээ алдсан бол тохирох тэнцвэржүүлэх үйлдлүүдийг хийнэ.
 */
void avl_del(AVL *ptree, int x)
{
        ptree -> root = delete_x ( ptree -> root , x ) ;
}

/*
  Хамгийн багыг устгах функц: ТМноос хамгийг бага утгыг нь устгах функц.
  Устгасан утгыг буцаана.
  Хэрэв мод тэнцвэрээ алдсан бол тохирох тэнцвэржүүлэх үйлдлүүдийг хийнэ.
 */
int avl_delMin(AVL *ptree)
{
        Elm *temp = min_val ( ptree -> root ) ;
        ptree -> root = delete_x ( ptree -> root , min ) ;
        return min ;
}

/*
  ТМыг inorder дарааллаар, нэг мөрөнд нэг утга хэвлэнэ.
 */
void avl_inorder(const AVL *ptree)
{
        in_order_p ( ptree -> root ) ;
}

/*
  ТМноос `x` утгатай оройг хайж олоод, тухайн оройд суурилсан
  дэд модонд хэдэн орой байгааг олж буцаана.
  Олдохгүй бол -1-ийг буцаана.
 */
int avl_size(const AVL *ptree, int x)
{
        struct Elm *temp = search_x ( ptree -> root , x ) ;
        if ( temp == NULL )
            return -1 ;
        return size_p ( temp ) ;
}

/*
  ТМноос `x` утгатай оройг хайж олоод, тухайн оройд суурилсан
  дэд модны өндөр хэд байгааг олж буцаана. Олдохгүй бол -1-ийг буцаана.
 */
int avl_height(const AVL *ptree, int x)
{
        struct Elm *temp = search_x ( ptree -> root , x ) ;
        if ( temp == NULL )
            return -1 ;
        return height_p ( temp ) ;
}
